# in-secure_ring

Para instalar OpenSSL:  

sudo yum update  
sudo yum install openssl o bien sudo yum install openssl-devel  
openssl version  

### Comandos para generar llaves 
openssl genpkey -algorithm RSA -out private_key.pem -pkeyopt rsa_keygen_bits:2048  
openssl rsa -pubout -in private_key.pem -out public_key.pem  

### Para crear autoridad certificadora:
1. Crear la estructura de directorios:  
Crea una estructura de directorios para almacenar los certificados y llaves relacionados con la Autoridad Certificadora. Por ejemplo:  
  
mkdir ~/ca  
cd ~/ca  
mkdir private certs newcerts crl  
chmod 700 private  
touch index.txt  
echo 1000 > serial  
  
Este conjunto de comandos crea los directorios private, certs, newcerts y crl en la carpeta ~/ca.   
Además, establece los permisos adecuados para el directorio private, crea un archivo de índice (index.txt) y un archivo de serie (serial). 
   
2. Generar la llave privada de la Autoridad Certificadora:  
Utiliza OpenSSL para generar una llave privada para la Autoridad Certificadora (AC). Ejecuta el siguiente comando:  
  
openssl genpkey -algorithm RSA -out private/ca.key.pem -aes256  
  
Este comando solicita una contraseña para generar la llave.  
Esto generará una clave privada RSA y la almacenará en el archivo private/ca.key.pem. Asegúrate de proteger esta clave con una contraseña segura.  
  
3. Generar el certificado de la Autoridad Certificadora:  
A partir de la llave privada generada, ahora puedes generar el certificado de la Autoridad Certificadora (AC). Ejecuta el siguiente comando:  
  
openssl req -new -x509 -days 3650 -key private/ca.key.pem -out certs/ca.cert.pem  
  
Esto generará un certificado autofirmado válido por 10 años y lo almacenará en el archivo certs/ca.cert.pem.  
Durante el proceso, se te pedirá que proporciones información sobre tu organización y la Autoridad Certificadora.  
  
4. Configurar la Autoridad Certificadora:  
Crea un archivo de configuración (ca.cnf) para especificar los detalles de configuración de la Autoridad Certificadora.  
Puedes usar un editor de texto para crear este archivo y agregar la siguiente configuración:  
  
[ca]  
default_ca = my_ca  
  
[my_ca]  
new_certs_dir = ./newcerts  
unique_subject = no  
certificate = ./certs/ca.cert.pem  
private_key = ./private/ca.key.pem  
serial = ./serial  
crlnumber = ./crlnumber  
database = ./index.txt  
default_days = 365  
default_md = sha256  
policy = my_policy  
  
[my_policy]  
commonName = supplied  
stateOrProvinceName = optional  
countryName = optional  
emailAddress = optional  
organizationName = supplied  
organizationalUnitName = optional  
  
Asegúrate de guardar el archivo con el nombre ca.cnf en el directorio raíz de tu Autoridad Certificadora (~/ca en este ejemplo).  
  
### ¿Cómo se firma?
1. Generar una solicitud de certificado:   
La entidad o el usuario que desea obtener un certificado de tu AC debe generar una solicitud de certificado (CSR, por sus siglas en inglés) que incluya su información y su clave pública. 
Pueden hacerlo utilizando OpenSSL con el siguiente comando:  
  
openssl req -new -key <clave_privada_entidad>.pem -passin pass:<contraseña> -out <solicitud_csr>.csr  
  
Reemplaza <clave_privada_entidad>.pem con la ruta y el nombre del archivo que contiene tu clave privada protegida con contraseña, <contraseña> con la contraseña de tu clave privada, y <solicitud_csr>.csr con el nombre deseado para el archivo de solicitud de certificado.  
  
Al utilizar la opción -passin pass:<contraseña>, se le indicará a OpenSSL que utilice la contraseña especificada para descifrar la clave privada protegida y generar la solicitud de certificado.  
  
Cuando ejecutes el comando, se te pedirá que ingreses la contraseña de tu clave privada. Una vez que la contraseña sea proporcionada correctamente, OpenSSL generará la solicitud de certificado en el archivo <solicitud_csr>.csr.  
  
2. Firmar el certificado:  
Utiliza el comando openssl ca para firmar el certificado utilizando tu AC.  
Asegúrate de tener la solicitud de certificado (<solicitud_csr>.csr) y la configuración de la AC (ca.cnf) en el mismo directorio. Ejecuta el siguiente comando:  
  
openssl ca -config ca.cnf -in <solicitud_csr>.csr -out <certificado>.crt  
  
Reemplaza <solicitud_csr>.csr con el nombre del archivo de solicitud de certificado y <certificado>.crt con el nombre que deseas para el archivo de certificado firmado.  
  
Durante el proceso, se te pedirá que ingreses la contraseña de tu clave privada de la AC y confirmes la firma del certificado.  
OpenSSL utilizará la configuración de tu AC (ca.cnf) para generar y firmar el certificado.  
  
3. Certificado firmado:  
Una vez que hayas ejecutado el comando, se generará el certificado firmado <certificado>.crt utilizando la clave privada de tu AC.  
Este certificado puede ser entregado a la entidad o usuario correspondiente.  