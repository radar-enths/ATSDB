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

/*
 * FilterGeneratorWidget.cpp
 *
 *  Created on: May 30, 2012
 *      Author: sk
 */

#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

#include "filtergeneratorwidget.h"
#include "atsdb.h"
#include "dbovariable.h"
#include "dbovariableselectionwidget.h"
#include "configurationmanager.h"
#include "filtermanager.h"
#include "stringconv.h"

using namespace Utils;

FilterGeneratorWidget::FilterGeneratorWidget(QWidget *parent)
 : QWidget (parent)
{
  condition_variable_widget_=0;
  setWindowTitle(tr("Add New Filter"));

  setMinimumSize (800, 400);

  createGUIElements ();
}

FilterGeneratorWidget::~FilterGeneratorWidget()
{

}

void FilterGeneratorWidget::createGUIElements ()
{
  QFont font_bold;
  font_bold.setBold(true);
  QFont font_big;
  font_big.setPointSize(18);

  QVBoxLayout *layout = new QVBoxLayout ();

  QHBoxLayout *name_layout = new QHBoxLayout ();
  QLabel *name_label = new QLabel (tr("Define unique filter name"));
  name_layout->addWidget(name_label);
  filter_name_ = new QLineEdit (tr("Filter0"));
  name_layout->addWidget(filter_name_);
  layout->addLayout (name_layout);

  QLabel *condition_label = new QLabel (tr("Define condition"));
  layout->addWidget(condition_label);

  QGridLayout *condition_layout = new QGridLayout ();

  QVBoxLayout *var_layout = new QVBoxLayout ();
  condition_variable_widget_ = new DBOVariableSelectionWidget ();
  var_layout->addWidget (condition_variable_widget_);
  var_layout->addStretch();
  condition_layout->addLayout (var_layout, 0, 0);

  QVBoxLayout *math_layout = new QVBoxLayout ();

  condition_absolute_ = new QCheckBox ("ABS");

  math_layout->addWidget (condition_absolute_);
  condition_layout->addLayout (math_layout, 0, 1);

  QVBoxLayout *operator_layout = new QVBoxLayout ();
  QLabel *label = new QLabel (tr("Operator"));
  label->setFont (font_bold);
  operator_layout->addWidget(label);

  condition_combo_=new QComboBox();
  condition_combo_->addItem("=");
  condition_combo_->addItem("!=");
  condition_combo_->addItem(">");
  condition_combo_->addItem(">=");
  condition_combo_->addItem("<");
  condition_combo_->addItem("<=");
  condition_combo_->addItem("|=");
  operator_layout->addWidget(condition_combo_);
  operator_layout->addStretch();

  condition_layout->addLayout (operator_layout, 0, 2);

  QVBoxLayout *value_layout = new QVBoxLayout ();
  QLabel *label2 = new QLabel (tr("Value"));
  label2->setFont (font_bold);
  value_layout->addWidget(label2);

  condition_value_ = new QLineEdit ();
  value_layout->addWidget(condition_value_);

  QPushButton *load_min = new QPushButton(tr("Load min"));
  connect(load_min, SIGNAL( clicked() ), this, SLOT( loadMin() ));
  value_layout->addWidget(load_min);

  QPushButton *load_max = new QPushButton(tr("Load max"));
  connect(load_max, SIGNAL( clicked() ), this, SLOT( loadMax() ));
  value_layout->addWidget(load_max);

  value_layout->addStretch();

  condition_layout->addLayout (value_layout, 0, 3);

  QVBoxLayout *reset_layout = new QVBoxLayout ();
  QLabel *label_reset = new QLabel (tr("Reset value"));
  label_reset->setFont (font_bold);
  reset_layout->addWidget(label_reset);

  condition_reset_combo_=new QComboBox();
  condition_reset_combo_->addItem("MIN");
  condition_reset_combo_->addItem("MAX");
  condition_reset_combo_->addItem("value");
  reset_layout->addWidget(condition_reset_combo_);
  reset_layout->addStretch();

  condition_layout->addLayout (reset_layout, 0, 4);

  layout->addLayout (condition_layout);

  QPushButton *add_condition = new QPushButton(tr("Add condition"));
  connect(add_condition, SIGNAL( clicked() ), this, SLOT( addCondition() ));
  layout->addWidget(add_condition);
  layout->addStretch();

  QLabel *conditions_label = new QLabel (tr("Current conditions"));
  conditions_label->setFont (font_bold);
  layout->addWidget(conditions_label);

  conditions_list_ = new QListWidget();
  layout->addWidget(conditions_list_);

  QHBoxLayout *button_layout = new QHBoxLayout ();

  QPushButton *cancel = new QPushButton(tr("Cancel"));
  connect(cancel, SIGNAL( clicked() ), this, SLOT( cancel() ));
  button_layout->addWidget(cancel);

  button_layout->addStretch();

  QPushButton *ok = new QPushButton(tr("Add"));
  connect(ok, SIGNAL( clicked() ), this, SLOT( accept() ));
  button_layout->addWidget(ok);

  layout->addLayout (button_layout);
  setLayout (layout);
}

//void FilterGeneratorWidget::loadMin ()
//{
//  assert (condition_variable_widget_);
//  DBOVariable *var = condition_variable_widget_->selectedVariable();

//  // TOFO FIX REPRESENTATION
//  assert (false);
////  std::string min = var->getRepresentationFromValue(ATSDB::getInstance().getMinAsString (var));
////  condition_value_->setText (tr(min.c_str()));
//}
//void FilterGeneratorWidget::loadMax ()
//{
//  assert (condition_variable_widget_);
//  DBOVariable *var = condition_variable_widget_->getSelectedVariable();

//  // TOFO FIX REPRESENTATION
//  assert (false);
////  std::string max = var->getRepresentationFromValue(ATSDB::getInstance().getMaxAsString (var));
////  condition_value_->setText (tr(max.c_str()));
//}

void FilterGeneratorWidget::addCondition ()
{
  assert (condition_variable_widget_);
  assert (condition_combo_);

  const DBOVariable &var = condition_variable_widget_->selectedVariable();

  ConditionTemplate data_condition;
  data_condition.variable_name_=var.name();
  data_condition.variable_dbo_type_=var.dboName();
  data_condition.absolute_value_=condition_absolute_->checkState() == Qt::Checked;
  data_condition.operator_=condition_combo_->currentText().toStdString();
  data_condition.value_=condition_value_->text().toStdString();
  data_condition.reset_value_=condition_reset_combo_->currentText().toStdString();

  data_conditions_.push_back (data_condition);
  updateWidgetList();
}

void FilterGeneratorWidget::updateWidgetList()
{
  assert (conditions_list_);
  conditions_list_->clear();

  for (unsigned int cnt=0; cnt < data_conditions_.size(); cnt++)
  {
    ConditionTemplate &data_condition = data_conditions_.at(cnt);
    std::string variable = data_condition.variable_name_;
    if (data_condition.absolute_value_)
      variable = "ABS("+variable+")";
    std::string text = variable+" "+data_condition.operator_+" "+data_condition.value_+", resets to " + data_condition.reset_value_;
    conditions_list_->addItem ( tr(text.c_str()) );
  }
}

void  FilterGeneratorWidget::closeEvent ( QCloseEvent * event )
{
  emit filterWidgetAction (false);
}

void FilterGeneratorWidget::accept ()
{
  std::string filter_name=filter_name_->text().toStdString();

  Configuration &configuration = ATSDB::instance().filterManager().addNewSubConfiguration ("DBFilter", filter_name);

  for (unsigned int cnt=0; cnt < data_conditions_.size(); cnt++)
  {
    ConditionTemplate &data_condition = data_conditions_.at(cnt);
    std::string condition_name = filter_name+"Condition"+String::intToString(cnt);

    Configuration &condition_configuration = configuration.addNewSubConfiguration ("DBFilterCondition", condition_name);
    condition_configuration.addParameterString ("operator", data_condition.operator_);
    condition_configuration.addParameterString ("variable_name", data_condition.variable_name_);
    condition_configuration.addParameterString ("variable_dbo_name", data_condition.variable_dbo_type_);
    condition_configuration.addParameterBool ("absolute_value", data_condition.absolute_value_);
    condition_configuration.addParameterString ("value", data_condition.value_);
    std::string reset_value;
    if (data_condition.reset_value_.compare("MIN") == 0 || data_condition.reset_value_.compare("MAX") == 0)
      reset_value = data_condition.reset_value_;
    else
      reset_value = data_condition.value_;
    condition_configuration.addParameterString ("reset_value", reset_value);

    //configuration.addSubConfigurable ("DBFilterCondition", condition_name, condition_config_name);
  }

  ATSDB::instance().filterManager().generateSubConfigurable ("DBFilter", filter_name);

  emit filterWidgetAction (true);
}

void FilterGeneratorWidget::cancel ()
{
  emit filterWidgetAction (false);
}
