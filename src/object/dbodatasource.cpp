/*
 * This file is part of ATSDB.
 *
 * ATSDB is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ATSDB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ATSDB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dbodatasource.h"
#include "dbobject.h"
#include "dbodatasourcedefinitionwidget.h"
#include "projectionmanager.h"

DBODataSourceDefinition::DBODataSourceDefinition(const std::string &class_id, const std::string &instance_id, DBObject* object)
    : Configurable (class_id, instance_id, object), object_(object)
{
    registerParameter ("schema", &schema_, "");
    registerParameter ("local_key", &local_key_, "");
    registerParameter ("meta_table", &meta_table_,  "");
    registerParameter ("foreign_key", &foreign_key_, "");
    registerParameter ("short_name_column", &short_name_column_, "");
    registerParameter ("name_column", &name_column_, "");
    registerParameter ("sac_column", &sac_column_, "");
    registerParameter ("sic_column", &sic_column_, "");
    registerParameter ("latitude_column", &latitude_column_, "");
    registerParameter ("longitude_column", &longitude_column_, "");
    registerParameter ("altitude_column", &altitude_column_, "");
}

DBODataSourceDefinition::~DBODataSourceDefinition()
{
    if (widget_)
    {
        delete widget_;
        widget_ = nullptr;
    }
}

DBODataSourceDefinitionWidget* DBODataSourceDefinition::widget ()
{
    if (!widget_)
    {
        widget_ = new DBODataSourceDefinitionWidget (*object_, *this);
    }

    assert (widget_);
    return widget_;
}

std::string DBODataSourceDefinition::latitudeColumn() const
{
    return latitude_column_;
}

std::string DBODataSourceDefinition::longitudeColumn() const
{
    return longitude_column_;
}

void DBODataSourceDefinition::longitudeColumn(const std::string &longitude_column)
{
    loginf << "DBODataSourceDefinition: localKey: value " << longitude_column;
    longitude_column_ = longitude_column;
    emit definitionChangedSignal();
}

std::string DBODataSourceDefinition::shortNameColumn() const
{
    return short_name_column_;
}

void DBODataSourceDefinition::shortNameColumn(const std::string &short_name_column)
{
    loginf << "DBODataSourceDefinition: shortNameColumn: value " << short_name_column;
    short_name_column_ = short_name_column;
    emit definitionChangedSignal();
}

std::string DBODataSourceDefinition::sacColumn() const
{
    return sac_column_;
}

void DBODataSourceDefinition::sacColumn(const std::string &sac_column)
{
    loginf << "DBODataSourceDefinition: sacColumn: value " << sac_column;
    sac_column_ = sac_column;
    emit definitionChangedSignal();
}

std::string DBODataSourceDefinition::sicColumn() const
{
    return sic_column_;
}

void DBODataSourceDefinition::sicColumn(const std::string &sic_column)
{
    loginf << "DBODataSourceDefinition: sicColumn: value " << sic_column;
    sic_column_ = sic_column;
    emit definitionChangedSignal();
}

std::string DBODataSourceDefinition::altitudeColumn() const
{
    return altitude_column_;
}

void DBODataSourceDefinition::altitudeColumn(const std::string &altitude_column)
{
    loginf << "DBODataSourceDefinition: altitudeColumn: value " << altitude_column;
    altitude_column_ = altitude_column;
    emit definitionChangedSignal();
}

void DBODataSourceDefinition::latitudeColumn(const std::string &latitude_column)
{
    loginf << "DBODataSourceDefinition: latitudeColumn: value " << latitude_column;
    latitude_column_ = latitude_column;
    emit definitionChangedSignal();
}

void DBODataSourceDefinition::localKey(const std::string &local_key)
{
    loginf << "DBODataSourceDefinition: localKey: value " << local_key;
    local_key_ = local_key;
    emit definitionChangedSignal();
}

void DBODataSourceDefinition::metaTable(const std::string &meta_table)
{
    loginf << "DBODataSourceDefinition: metaTable: value " << meta_table;
    meta_table_ = meta_table;
    emit definitionChangedSignal();
}

void DBODataSourceDefinition::foreignKey(const std::string &foreign_key)
{
    loginf << "DBODataSourceDefinition: foreignKey: value " << foreign_key;
    foreign_key_ = foreign_key;
    emit definitionChangedSignal();
}

void DBODataSourceDefinition::nameColumn(const std::string &name_column)
{
    loginf << "DBODataSourceDefinition: nameColumn: value " << name_column;
    name_column_ = name_column;
    emit definitionChangedSignal();
}

DBODataSource::DBODataSource(unsigned int id, const std::string& name)
: id_(id), name_(name)
{
}

DBODataSource::~DBODataSource()
{

}

void DBODataSource::finalize ()
{
    finalized_ = false;

    ProjectionManager& proj_man = ProjectionManager::instance();

    assert (proj_man.useOGRProjection() || proj_man.useSDLProjection());

    if (proj_man.useOGRProjection())
    {
        bool ret = proj_man.ogrGeo2Cart(latitude_, longitude_, ogr_system_x_, ogr_system_y_);

        if (!ret)
        {
           logwrn << "DBODataSource: finalize: ogrGeo2Cart return false for " << short_name_
                  << " lat " << latitude_ << " lon " << longitude_ << " x " << ogr_system_x_ << " y " << ogr_system_y_;
           return;
        }
    }

    if (proj_man.useSDLProjection())
    {
        preset_cpos (&grs_pos_);
        preset_gpos (&geo_pos_);
        preset_mapping_info (&mapping_info_);

        geo_pos_.latitude = latitude_;
        geo_pos_.longitude = longitude_;
        geo_pos_.altitude = altitude_; // TODO check if exists
        geo_pos_.defined = true;

        t_Retc lrc;

        lrc = geo_calc_info (geo_pos_, &mapping_info_);
        assert (lrc == RC_OKAY);
    }

    loginf << "DBODataSource: finalize: " << short_name_ << " done";

    finalized_ = true;
}

// azimuth degrees, range & altitude in meters
void DBODataSource::calculateOGRSystemCoordinates (double azimuth, double slant_range, double altitude,
                                                   bool has_altitude, double &sys_x, double &sys_y)
{
    if (!finalized_)
        logerr << "DBODataSource: calculateSystemCoordinates: " << short_name_ << " not finalized";

    assert (finalized_);

    double range;

//    if (slant_range <= altitude)
//    {
//        logerr << "DataSource: calculateSystemCoordinates: a " << azimuth << " sr " << slant_range << " alt " << altitude
//                << ", assuming range = slant range";
//        range = slant_range; // TODO pure act of desperation
//    }
//    else
//        range = sqrt (slant_range*slant_range-altitude*altitude); // TODO: flatland

    if (has_altitude && slant_range > altitude)
        range = sqrt (slant_range*slant_range-altitude*altitude);
    else
        range = slant_range; // TODO pure act of desperation

    azimuth *= DEG2RAD;

    sys_x = range * sin (azimuth);
    sys_y = range * cos (azimuth);

    sys_x += ogr_system_x_;
    sys_y += ogr_system_y_;

    if (sys_x != sys_x || sys_y != sys_y)
    {
        logerr << "DBODataSource: calculateSystemCoordinates: a " << azimuth << " sr " << slant_range << " alt " << altitude
                << " range " << range << " sys_x " << sys_x << " sys_y " << sys_y;
        throw std::runtime_error ("DBODataSource: calculateSystemCoordinates: failed transformation");
    }
}

double DBODataSource::altitude() const
{
    return altitude_;
}

unsigned int DBODataSource::id() const
{
    return id_;
}

double DBODataSource::latitude() const
{
    return latitude_;
}

double DBODataSource::longitude() const
{
    return longitude_;
}

bool DBODataSource::hasShortName() const
{
    return has_short_name_;
}

bool DBODataSource::hasSac() const
{
    return has_sac_;
}

bool DBODataSource::hasSic() const
{
    return has_sic_;
}

bool DBODataSource::hasLatitude() const
{
    return has_latitude_;
}

bool DBODataSource::hasLongitude() const
{
    return has_longitude_;
}

bool DBODataSource::hasAltitude() const
{
    return has_altitude_;
}

const std::string &DBODataSource::name() const
{
    return name_;
}

unsigned char DBODataSource::sac() const
{
    return sac_;
}

const std::string &DBODataSource::shortName() const
{
    return short_name_;
}

unsigned char DBODataSource::sic() const
{
    return sic_;
}

void DBODataSource::altitude(double altitude)
{
    this->altitude_ = altitude;
}

void DBODataSource::latitude(double latitiude)
{
    this->latitude_ = latitiude;
}

void DBODataSource::longitude(double longitude)
{
    this->longitude_ = longitude;
}

void DBODataSource::sac(unsigned char sac)
{
    this->sac_ = sac;
}

void DBODataSource::shortName(const std::string &short_name)
{
    this->short_name_ = short_name;
}

void DBODataSource::sic(unsigned char sic)
{
    this->sic_ = sic;
}
