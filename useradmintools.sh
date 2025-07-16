#!/bin/bash
################################################################################
# Begin $usr/bin/useradmintools
# Description : This script is used to administer users, add, delete, etc.
#               It only creates users of the spadmin or spuser type.
#
#                (c) Copyright 2010, Spectracom Corporation
#                           ALL RIGHTS RESERVED
################################################################################

################################################################################
# Variables and Constants
################################################################################

: "${USER_HOME:="/home/spectracom"}"

################################################################################
# Functions
################################################################################

function set_user_permissions
{
    usermod -g spuser -G spuser "${1}"
}

function set_administrator_permissions
{
    usermod -g spadmin -G spuser,spadmin "${1}"
}

function is_user_admin
{
    # returns true if user is admin-only (not factory)
    local group
    local is_admin
    local is_factory

    # spaces on each sides to make following regex easier
    group=" $(groups "$1") "

    # spaces are here to prevent matching a substring of another
    # group's name that might contain `spadmin` or `spfactory`
    # these string are empty if the group is in the group list
    is_admin="${group##* spadmin *}"
    is_factory="${group##* spfactory *}"

    [ -z "${is_admin}" ] && [ -n "${is_factory}" ]
}

function admin_number
{
    # returns the number of admin-only (not factory) account
    local factory
    local admin
    local ret
    # filter spadmin and spfactory users
    readarray tmp <<<"$(sed -n -e 's/spadmin:.*:.*://p' -e 's/spfactory:.*:.*://p' /etc/group)"
    # only keep `,` characters because it's simpler for counting
    factory=${tmp[0]//[^,]/}
    admin=${tmp[1]//[^,]/}
    # number of spadmin - spfactory users by substracting the number of `,`
    ret=$(( ${#admin} - ${#factory} ))
    # absolute value
    echo ${ret#-}
}

################################################################################
# MAIN Bash Script Body
################################################################################

case "${1}" in
    add_spuser)
        useradd -M -N -g spuser -d "${USER_HOME}" "${2}"
        ;;

    add_spadmin)
        useradd -M -N -g spadmin -G spuser,spadmin -d "${USER_HOME}" "${2}"
        ;;

    set_username)
        usermod -l "${3}" "${2}"
        ;;

    set_group_spuser)
        set_user_permissions "${2}"
        ;;

    set_group_spadmin)
        set_administrator_permissions "${2}"
        ;;

    # set_passwd)
    #    pamsetpasswd "${2}" "${3}"
    #    ;;

    delete_user)
        # if the user to be deleted is the last spadmin-only user
        if is_user_admin "${2}" && [ "$(admin_number)" -le 1 ]
        then
            echo "ERROR: deleting last admin account"
            exit 126
        fi
        # kill all the processes that are started by that user
        pkill -u "${2}"
        userdel "${2}"
        ;;

    # temporary version of the password history file, to prevent password reuse
    create_pass_hist_temp)
        touch /etc/security/nopasswd
        chown root:root /etc/security/nopasswd
        chmod 660 /etc/security/nopasswd
        ;;

    *)
        echo "USAGE: ${0} {add_spuser|add_spadmin|set_username|set_group_spuser|set_group_spadmin|delete_user}."
        exit 1
        ;;
esac