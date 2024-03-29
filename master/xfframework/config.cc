
#include "config.h"
#include <iostream>
#include <sstream>

XfConfig::~XfConfig()
{
    if (_save_file)
        g_key_file_save_to_file(_config_key_file, _config_file_path.c_str(),
                                nullptr);
    if (_config_key_file)
    {
        g_key_file_free(_config_key_file);
    }
}

bool XfConfig::Open(std::string config_file_path)
{
    GError *error = NULL;
    // open ini file
    _config_key_file = g_key_file_new();
    if (!g_key_file_load_from_file(
            _config_key_file, config_file_path.c_str(),
            (GKeyFileFlags)(G_KEY_FILE_KEEP_COMMENTS |
                            G_KEY_FILE_KEEP_TRANSLATIONS),
            &error))
    {
        XF_LOGT(ERROR, TAG, "Error loading %s config file: %s\n",
                config_file_path.c_str(), error->message);

        g_error_free(error);
        error = NULL;
        return false;
    }
    _config_file_path = config_file_path;

    return true;
}

bool XfConfig::HasGroup(std::string group) const
{
    return g_key_file_has_group(_config_key_file, group.c_str());
}

bool XfConfig::HasKey(std::string group, std::string key) const
{
    GError *error = NULL;
    gboolean ret = g_key_file_has_key(_config_key_file, group.c_str(),
                                      key.c_str(), &error);
    if (!ret)
    {
        XF_LOGT(ERROR, TAG, "get group: %s key: %s error %s\n", group.c_str(),
                key.c_str(), error->message);
        // g_print("get group: %s key: %s error %s\n", group.c_str(),
        // key.c_str(), error->message);

        g_error_free(error);
        error = NULL;
    }

    return ret;
}

bool XfConfig::GetKeys(std::string group, std::vector<std::string> &keys) const
{
    GError *error = nullptr;
    gchar **gkeys = nullptr;
    gchar **gkey = nullptr;

    gsize key_nb;
    gkeys =
        g_key_file_get_keys(_config_key_file, group.c_str(), &key_nb, &error);
    if (!gkeys)
    {
        XF_LOGT(ERROR, TAG, "get group all keys: %s error %s\n", group.c_str(),
                error->message);
        // g_print("get group all keys: %s error %s\n", group.c_str(),
        // error->message);

        g_error_free(error);
        error = NULL;
        return false;
    }
    for (gkey = gkeys; *gkey; gkey++)
    {
        keys.push_back(*gkey);
    }

    g_strfreev(gkeys);

    return true;
}

bool XfConfig::GetGroups(std::vector<std::string> &groups) const
{
    gsize group_nb;
    gchar **ggroups = nullptr;
    gchar **ggroup = nullptr;

    ggroups = g_key_file_get_groups(_config_key_file, &group_nb);
    if (!ggroups)
    {
        XF_LOGT(ERROR, TAG, "get all groups  error!\n");
        // g_print("get all groups  error!\n");
        return false;
    }
    for (ggroup = ggroups; *ggroup; ggroup++)
    {
        groups.push_back(*ggroup);
    }

    g_strfreev(ggroups);
    return true;
}

std::vector<std::string> XfConfig::_Split(std::string str, std::string part)
{
    std::string strs = str + part;
    std::size_t previous = 0;
    std::size_t current = str.find(part);
    std::vector<std::string> elems;
    while (current != std::string::npos)
    {
        if (current != previous)
            elems.push_back(strs.substr(previous, current - previous));
        previous = current + 1;
        current = strs.find(part, previous);
    }
    return std::move(elems);
}

bool XfConfig::RemoveGroup(std::string group)
{
    GError *error = nullptr;
    gboolean ret =
        g_key_file_remove_group(_config_key_file, group.c_str(), &error);
    if (!ret)
    {
        XF_LOGT(ERROR, TAG, "remove group: %s error %s\n", group.c_str(),
                error->message);
        // g_print("remove group: %s error %s\n", group.c_str(),
        // error->message);
        g_error_free(error);
        error = NULL;
        return false;
    }
    if (!_save_file)
        _save_file = true;
    return true;
}

bool XfConfig::RemoveKey(std::string group, std::string key)
{
    GError *error = nullptr;
    gboolean ret = g_key_file_remove_key(_config_key_file, group.c_str(),
                                         key.c_str(), &error);
    if (!ret)
    {
        XF_LOGT(ERROR, TAG, "remove group: %s key %s error %s\n", group.c_str(),
                key.c_str(), error->message);
        // g_print("remove group: %s key %s error %s\n", group.c_str(),
        // key.c_str(), error->message);
        g_error_free(error);
        error = NULL;
        return false;
    }
    if (!_save_file)
        _save_file = true;
    return true;
}