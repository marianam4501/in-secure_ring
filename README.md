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
  
openssl req -new -key private/<clave_privada_entidad>.pem -passin pass:<contraseña> -out newcerts/<solicitud_csr>.csr    
  
Reemplaza <clave_privada_entidad>.pem con la ruta y el nombre del archivo que contiene tu clave privada protegida con contraseña, <contraseña> con la contraseña de tu clave privada, y <solicitud_csr>.csr con el nombre deseado para el archivo de solicitud de certificado.  
  
Al utilizar la opción -passin pass:<contraseña>, se le indicará a OpenSSL que utilice la contraseña especificada para descifrar la clave privada protegida y generar la solicitud de certificado.  
  
Cuando ejecutes el comando, se te pedirá que ingreses la contraseña de tu clave privada. Una vez que la contraseña sea proporcionada correctamente, OpenSSL generará la solicitud de certificado en el archivo <solicitud_csr>.csr.  
  
2. Firmar el certificado:  
Utiliza el comando openssl ca para firmar el certificado utilizando tu AC.  
Asegúrate de tener la solicitud de certificado (<solicitud_csr>.csr) y la configuración de la AC (ca.cnf) en el mismo directorio. Ejecuta el siguiente comando:  
  
openssl ca -config ca.cnf -in newcerts/<solicitud_csr>.csr -out certs/<certificado>.crt    
  
Reemplaza <solicitud_csr>.csr con el nombre del archivo de solicitud de certificado y <certificado>.crt con el nombre que deseas para el archivo de certificado firmado.  
  
Durante el proceso, se te pedirá que ingreses la contraseña de tu clave privada de la AC y confirmes la firma del certificado.  
OpenSSL utilizará la configuración de tu AC (ca.cnf) para generar y firmar el certificado.  
  
3. Certificado firmado:  
Una vez que hayas ejecutado el comando, se generará el certificado firmado <certificado>.crt utilizando la clave privada de tu AC.  
Este certificado puede ser entregado a la entidad o usuario correspondiente.  

openssl req -new -key mariana_key.pem -out fabian.csr
openssl req -new -key private_key.pem -out newcerts/mariana.csr
openssl ca -config ca.cnf -in ../fabian.csr -out certs/fabian.crt

## Flujo CDCD
Paso 1: Se agrega un mensaje al directorio donde se almacenan los mensajes a enviar por el canal en la máquina desde la cual se van a enviar. 
  
Paso 2: Se corre el programa que va a recibir en una máquina primero, luego las máquinas intermedias y, por último, la máquina que va a enviar los mensajes.  

El programa recibe 3 argumentos:

Primer argumento:
's' - sender (indica que es la computadora donde salen los mensajes)
'm' - middle (indica que es una computadora que reenvia los mensajes)
'r' - receiver (indica que es la computadora que recibe los mensajes)

Segundo argumento:
Ip de la computadora actual

Tercer argumento:
Ip de la computadora siguiente

Se debe indicar la IP del cliente y del servidor para cada máquina. En las máquinas que reciben mensajes, solo es necesario incluir la IP por donde se van a recibir los mensajes.
En el caso de las máquinas intermediarias y la que envia los mensajes, es necesario agregar ambas direcciones IP.

Paso 3: Una vez que ya se corren los programas en las distintas máquinas, se empiezan a enviar los mensajes.  
La máquina que envía, lee un mensaje del directorio y lo encripta. Una vez encriptado, el mensaje es enviado a la siguiente máquina. Las máquinas intermediarias lo reciben y lo reenvian a la siguiente.  

Paso 4: Cuando el mensaje llega al final del anillo, a la máquina destino, el mensaje es desencriptado y guardado en el directorio correspondiente.

Se puede utilizar make para acelerar el proceso:

make 1 - primera máquina
make 2 - segunda máquina
make 3 - tercera máquina
make 4 - cuarta máquina
make 5 - quinta máquina

## Flujo EAEA
Paso 1: Un usuario ingresa un nombre, un sha y un mensaje. El sha y el mensaje van firmados con el fin de confirmar la identidad de la persona (autenticación).  
Paso 2: El programa lee el archivo y verifica la identidad de la persona. Verifica si es quien dice ser. Se desencripta el mensaje y el sha.  
Paso 3: Se verifica que el mensaje no haya sido modificado. Se utiliza el algoritmo sha para hashear el mensaje en texto y se compara con el sha para garantizar que no haya sido modificado.  
Paso 4: Una vez realizadas esas verificaciones, se reenvía el mensaje por la red y se repite el paso 3 en cada máquina.  
Paso 5: Cuando el mensaje llega a la última máquina y se realizan las verificaciones correspondientes, si no hay ningun problema de integridad, el mensaje se guarda en el directorio correspondiente. 

Se puede utilizar make para acelerar el proceso:

make 11 - primera máquina
make 22 - segunda máquina
make 33 - tercera máquina
make 44 - cuarta máquina
make 55 - quinta máquina

### ¿Qué debo hacer para verificar la identidad de la persona?
Una vez creada la autoridad certificadora, firmo las peticiones de certificados que enviaron los compañeros. Esos certificados se suben en un zip a mediación de manera que todos tengan acceso a sus certificados y puedan ingresar mensajes para ser procesados.  
En el programa:  
  -> Autenticación del certificado. El sistema receptor verifica la autenticidad del certificado mediante una serie de pasos:  
  
a. Verificación de la firma digital: El sistema utiliza la clave pública de la CA que emitió el certificado para verificar la firma digital del certificado. Si la firma es válida, esto indica que el certificado no ha sido alterado desde su emisión.
  
b. Comprobación de la validez del certificado
  
  -> Establecimiento de la conexión segura: Si el certificado pasa todas las verificaciones anteriores, se considera válido y confiable. El sistema receptor y la entidad solicitante pueden establecer una conexión segura utilizando algoritmos criptográficos basados en las claves públicas y privadas del certificado.

### Comandos de OpenSSL
- Verificar la validez de un certificado:  
  
openssl verify -CAfile ca_cert.pem certificate.pem
  
- Verificar el contenido de un certificado:  
  
openssl x509 -in certificate.pem -text -noout
  
- Verificar la cadena de confianza de un certificado:  
  
openssl verify -CAfile ca_chain.pem certificate.pem  
  
