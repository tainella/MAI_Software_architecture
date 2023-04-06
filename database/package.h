#ifndef PACKAGE_H
#define PACKAGE_H

#include "delivery.h"

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Package {
        private:
            long _id;
            long _user_id;
            long _delivery_id;
            int _weight;
            int _volume;
            bool _is_fragile;
            std::string _contains;

        public:

            static Package fromJSON(const std::string & str);

            long get_id() const;
            long get_user_id() const;
            long get_delivery_id() const;
            int get_weight() const;
            int get_volume() const;
            bool get_is_fragile() const;
            const std::string &get_contains() const;

            //для редактирования
            long& id();
            long& user_id();
            long& delivery_id();
            int &weight();
            int &volume();
            bool &is_fragile();
            std::string &contains();

            static void init();
            static std::optional<Package> read_by_id(long id);
            static std::vector<Package> read_by_user_id(long user_id);
            static std::vector<Package> read_all();
            static int delete_package(long id);
            static std::optional<Delivery> get_delivery_data(long id);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif