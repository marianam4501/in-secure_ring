# in-secure_ring

Para instalar OpenSSL:

sudo yum update
sudo yum install openssl o bien sudo yum install openssl-devel
openssl version

### Comandos para generar llaves 
openssl genpkey -algorithm RSA -out private_key.pem -pkeyopt rsa_keygen_bits:2048
openssl rsa -pubout -in private_key.pem -out public_key.pem