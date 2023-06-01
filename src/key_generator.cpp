#include <iostream>
#include <fstream>
#include <openssl/rsa.h>
#include <openssl/pem.h>

int main(int argc, char* argv[])
{
    /*
    Eventualmente, se me ocurre que se podria pasar por parametro la ruta donde queremos
    que se guarden estos archivos, una ruta en la jerarquia de archivos que tenga la debida
    seguridad ya configurada.
    */
    const int keySize = 2048; // Tamaño de la llave en bits
    const char* privKeyFile = "private_key.pem"; // Archivo de la llave privada
    const char* pubKeyFile = "public_key.pem"; // Archivo de la llave pública
    
    /*
    RSA_generate_key(RSA *rsa, int bits, unsigned long e, void (*callback)(int,int,void *), void *cb_arg);
        rsa: apuntador a la estructura RSA que almacenará las llaves generadas.
        bits: tamaño de la llave en bits.
        e: exponente de cifrado. Por lo general, se utiliza el valor 65537.
        callback: función de callback que puede ser usada para monitorear el proceso de generación de llaves.
        cb_arg: argumento opcional para la función de callback.
    */

    // Generar la llave RSA
    RSA* rsa = RSA_generate_key(keySize, RSA_F4, nullptr, nullptr);
    
    // Guardar la llave privada en un archivo
    FILE* privKey = fopen(privKeyFile, "wb");
    if (privKey == nullptr) {
        std::cerr << "Error al abrir el archivo de la llave privada." << std::endl;
        return -1;
    }
    PEM_write_RSAPrivateKey(privKey, rsa, nullptr, nullptr, 0, nullptr, nullptr);
    fclose(privKey);
    
    // Guardar la llave pública en un archivo
    FILE* pubKey = fopen(pubKeyFile, "wb");
    if (pubKey == nullptr) {
        std::cerr << "Error al abrir el archivo de la llave pública." << std::endl;
        return -1;
    }
    PEM_write_RSAPublicKey(pubKey, rsa);
    fclose(pubKey);
    
    // Liberar la memoria de la llave RSA
    RSA_free(rsa);
    
    std::cout << "Llaves generadas exitosamente." << std::endl;
    
    return 0;
}
