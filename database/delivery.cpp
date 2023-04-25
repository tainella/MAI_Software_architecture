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
            select << "SELECT id, login_sender, login_receiver, adress, datetime FROM Delivery where id=?",
                into(a._id),
                into(a._login_sender),
                into(a._login_receiver),
                into(a._adress),
                into(a._datetime),
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

    std::vector<Delivery> Delivery::read_by_reciever(std::string login_receiver) {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Delivery a;
            std::vector<Delivery> result;

            select << "SELECT id, login_sender, login_receiver, adress, datetime FROM Delivery where login_receiver=?",
                into(a._id),
                into(a._login_sender),
                into(a._login_receiver),
                into(a._adress),
                into(a._datetime),
                use(login_receiver),
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

    std::vector<Delivery> Delivery::read_by_sender(std::string login_sender) {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Delivery a;
            std::vector<Delivery> result;

            select << "SELECT id, login_sender, login_receiver, adress, datetime FROM Delivery where login_sender=?",
                into(a._id),
                into(a._login_sender),
                into(a._login_receiver),
                into(a._adress),
                into(a._datetime),
                use(login_sender),
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

    std::vector<Delivery> Delivery::read_all() {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            Delivery a;
            std::vector<Delivery> result;

            select << "SELECT id, login_sender, login_receiver, adress, datetime FROM Delivery",
                into(a._id),
                into(a._login_sender),
                into(a._login_receiver),
                into(a._adress),
                into(a._datetime),
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

    std::vector<Package> Delivery::get_packages(long id) {

    }

    int Delivery::delete_delivery(long id) {

    };

    void Delivery::save_to_mysql()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Delivery (id, login_sender, login_receiver, adress, datetime) VALUES(?, ?, ?, ?, ?)",
                use(_id),
                use(_login_sender),
                use(_login_receiver),
                use(_adress),
                use(_datetime),
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

    long Delivery::get_id() const {
        return _id;
    };

    const std::string &Delivery::get_login_sender() const {
        return _login_sender;
    };

    const std::string &Delivery::get_login_receiver() const {
        return _login_receiver;
    };

    const std::string &Delivery::get_adress() const {
        return _adress;
    };

    const std::string &Delivery::get_datetime() const {
        return _datetime;
    };


    long &Delivery::id() {
        return _id;
    };

    std::string &Delivery::login_sender(){
        return _login_sender;
    };

    std::string &Delivery::login_receiver() {
        return _login_receiver;
    };

    std::string &Delivery::adress() {
        return _adress;
    };

    std::string &Delivery::datetime() {
        return _datetime;
    };

}
