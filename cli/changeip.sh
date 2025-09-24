nmcli con mod eth ipv4.addresses 10.1.10.206/24
nmcli con mod eth ipv4.gateway 10.1.10.1
nmcli con mod eth ipv4.dns "8.8.8.8"
nmcli con mod eth ipv4.method manual
nmcli con up eth
