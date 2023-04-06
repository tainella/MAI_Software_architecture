#include "package.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Package::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Package` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`user_id` INT NOT NULL"
                        << "`delivery_id` INT NOT NULL"
                        << "`weight` INT NOT NULL"
                        << "`volume` INT NOT NULL"
                        << "is_fragile BOOL NOT NULL"
                        << "`contains` VARCHAR(1024) NULL,"
                        << "FOREIGN KEY (`user_id`) REFERENCES User (id)"
                        << "FOREIGN KEY (`delivery_id`) REFERENCES Delivery (id)"
                        << "PRIMARY KEY (`id`);",
                now;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Package::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        root->set("delivery_id", _delivery_id);
        root->set("weight", _weight);
        root->set("volume", _volume);
        root->set("is_fragile", _is_fragile);
        root->set("contains", _contains);

        return root;
    }

    Package Package::fromJSON(const std::string &str)
    {
        Package pack;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        pack.id() = object->getValue<long>("id");
        pack.user_id() = object->getValue<long>("user_id");
        pack.delivery_id() = object->getValue<long>("delivery_id");
        pack.weight() = object->getValue<int>("weight");
        pack.volume() = object->getValue<int>("volume");
        pack.is_fragile() = object->getValue<bool>("is_fragile");
        pack.contains() = object->getValue<std::string>("contains");

        return pack;
    }

    std::optional<Package> Package::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Package a;
            select << "SELECT id, user_id, delivery_id, weight, volume, is_fragile, contains  FROM User where id=?",
                into(a._id),
                into(a._user_id),
                into(a._delivery_id),
                into(a._weight),
                into(a._volume),
                into(a._is_fragile),
                into(a._contains),
                use(id),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return a;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            
        }
        return {};
    }

    std::vector<Package> Package::read_by_user_id(long user_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            std::vector<Package> result;
            Package a;
            select << "SELECT id, user_id, delivery_id, weight, volume, is_fragile, contains  FROM User where id=?",
                into(a._id),
                into(a._user_id),
                into(a._delivery_id),
                into(a._weight),
                into(a._volume),
                into(a._is_fragile),
                into(a._contains),
                use(user_id),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {
            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }

    std::vector<Package> Package::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Package> result;
            Package a;
            select << "SELECT id, user_id, delivery_id, weight, volume, is_fragile, contains FROM User",
                into(a._id),
                into(a._user_id),
                into(a._delivery_id),
                into(a._weight),
                into(a._volume),
                into(a._is_fragile),
                into(a._contains),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    int Package::delete_package(long id) {

    }

    std::optional<Delivery> Package::get_delivery_data(long id) {
        
    }

    void Package::save_to_mysql()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO User (user_id, delivery_id, weight, volume, is_fragile, contains) VALUES(?, ?, ?, ?, ?, ?)",
                use(_user_id),
                use(_delivery_id),
                use(_weight),
                use(_volume),
                use(_is_fragile),
                use(_contains);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Package::get_id() const {
        return _id;
    };

    long Package::get_user_id() const {
        return _user_id;
    };

    long Package::get_delivery_id() const {
        return _delivery_id;
    };

    int Package::get_weight() const {
        return _weight;
    };

    int Package::get_volume() const{
        return _volume;
    };

    bool Package::get_is_fragile() const {
        return _is_fragile;
    };
    
    const std::string &Package::get_contains() const {
        return _contains;
    };

    long &Package::id() {
        return _id;
    };
    long &Package::user_id() {
        return _user_id;
    };
    long &Package::delivery_id() {
        return _delivery_id;
    };
    int &Package::weight() {
        return _weight;
    };
    int &Package::volume() {
        return _volume;
    };
    bool &Package::is_fragile() {
        return _is_fragile;
    };
    std::string &Package::contains() {
        return _contains;
    };
}