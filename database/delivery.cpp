#include "delivery.h"
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
    void Delivery::init()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Delivery` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`_login_sender` VARCHAR(1024) NOT NULL"
                        << "`_login_receiver` VARCHAR(1024) NOT NULL"
                        << "`_adress` VARCHAR(1024) NOT NULL"
                        << "`_datetime` VARCHAR(1024) NOT NULL"
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

    Poco::JSON::Object::Ptr Delivery::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("login_sender", _login_sender);
        root->set("login_receiver", _login_receiver);
        root->set("adress", _adress);
        root->set("datetime", _datetime);
        return root;
    }

    Delivery Delivery::fromJSON(const std::string &str)
    {
        Delivery deliv;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        deliv.id() = object->getValue<long>("id");
        deliv.login_sender() = object->getValue<std::string>("login_sender");
        deliv.login_receiver() = object->getValue<std::string>("login_receiver");
        deliv.adress() = object->getValue<std::string>("adress");
        deliv.datetime() = object->getValue<std::string>("datetime");
        return deliv;
    }

    std::optional<Delivery> Delivery::read_by_id(long id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Delivery a;
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


}
