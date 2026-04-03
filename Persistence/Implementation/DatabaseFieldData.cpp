#include "google/protobuf/util/json_util.h"
#include "spdlog/spdlog.h"

#include <DatabaseFieldData.h>
#include <fstream>
#include <utility>

DatabaseFieldData::DatabaseFieldData(FieldKey fieldKey, std::string className,
    std::unique_ptr<google::protobuf::Message> defaultFieldValue,
    std::function<bool(google::protobuf::Message*)> updateHandler)
        : fieldKey(fieldKey), fieldName(std::move(className)), defaultFieldValue(std::move(defaultFieldValue)), updateHandler(std::move(updateHandler)){

}

google::protobuf::util::JsonParseOptions DatabaseFieldData::parseOpts = {
    .allow_legacy_nonconformant_behavior = false,
    .ignore_unknown_fields = false,
    .case_insensitive_enum_parsing = true
};

bool DatabaseFieldData::PreUpdateField(google::protobuf::Message* newData) {
    if (updateHandler != nullptr) {
        return updateHandler(newData);
    }
    return true;
}

const std::string& DatabaseFieldData::GetFieldName() const {
    return fieldName;
}

const google::protobuf::Message* DatabaseFieldData::GetDefaultValue() const {
    return defaultFieldValue.get();
}

const FieldKey& DatabaseFieldData::GetFieldKey() const {
    return fieldKey;
}