# How to build
- Install mysql connector lib:
```bash
sudo apt-get install libmysqlcppconn-dev
sudo apt install libmysqlclient-dev
```
- Build:
```bash
.\build.sh
```


# My SQL configuration
- Install mysql server:
```bash
sudo apt install mysql-server
```
- Start the server
```bash
sudo systemctl start mysql.service
```
or, if error:
```bash
sudo service mysql start
```
- mysql -u root -p
