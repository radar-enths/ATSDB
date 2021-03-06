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

#ifndef INSERTBUFFERDBJOB_H_
#define INSERTBUFFERDBJOB_H_

#include <list>
#include "job.h"

class Buffer;
class DBObject;
class DBInterface;
class DBOVariable;
class DBTable;

/**
 * @brief Buffer write job
 *
 * Writes buffer's data contents to a database table.
 */
class InsertBufferDBJob : public Job
{
    Q_OBJECT

signals:
    void insertProgressSignal (float percent);

public:
    InsertBufferDBJob(DBInterface& db_interface, DBObject& dbobject, std::shared_ptr<Buffer> buffer,
                      bool emit_change=true);

    virtual ~InsertBufferDBJob();

    virtual void run ();

    std::shared_ptr<Buffer> buffer () { assert (buffer_); return buffer_; }

    bool emitChange() const;

protected:
    DBInterface& db_interface_;
    DBObject& dbobject_;
    std::shared_ptr<Buffer> buffer_;
    bool emit_change_ {true};

    void partialInsertBuffer (DBTable& table);
};

#endif /* INSERTBUFFERDBJOB_H_ */
