#ifndef DELIVERY_H
#define DELIVERY_H

#include "package.h"

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Delivery {
        private:
            long _id;
            std::string _login_sender;
            std::string _login_receiver;
            std::string _adress;
            std::string _datetime;

        public:
            static Delivery fromJSON(const std::string & str);
            // Delivery(); //?

            long get_id() const;
            std::string &get_login_sender() const;
            std::string &get_login_receiver() const;
            std::string &get_adress() const;
            std::string &get_datetime() const;

            //для редактирования
            long &id;
            std::string &login_sender;
            std::string &login_receiver;
            std::string &adress;
            std::string &datetime;

            static void init();
            static std::optional<Delivery> read_by_id(long id);
            static std::vector<Delivery> read_by_reciever(std::string login_receiver);
            static std::vector<Delivery> read_by_sender(std::string login_sender);
            static std::vector<Delivery> read_all();
            static std::vector<Package> get_packages(long id);
            static int delete_delivery(long id);

            void save_to_mysql();
            Poco::JSON::Object::Ptr toJSON() const;
    };
}

#endif