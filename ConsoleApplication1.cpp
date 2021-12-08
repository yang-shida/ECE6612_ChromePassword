// https://www.alertra.com/blog/decrypting-browser-passwords-other-secrets

#include <iostream>
#include "windows.h"
#include "dpapi.h"
#include <cstdio>
#include "base64.h"
#include <fstream>

using namespace std;

int main()
{
    // read in internal key (base64 encoded and DPAPI encrypted)
    ifstream fs;
    fs.open("crack_chrome.txt");
    if(!fs.is_open())
    {
        cout << "Cannot open key file" << endl;
        return 1;
    }
    string internal_key_base64;
    getline(fs, internal_key_base64);
    fs.close();

    cout << internal_key_base64.length() << endl;

    // base64 decode
    string internal_key_base64_decoded = base64_decode(internal_key_base64);
    cout << internal_key_base64_decoded.length() << endl;
    // strip off DPAPI
    internal_key_base64_decoded = internal_key_base64_decoded.substr(5);
    cout << internal_key_base64_decoded.length() << endl;

    // decrypt DPAPI
    DATA_BLOB DataOut;
    DataOut.pbData =
        const_cast<BYTE*>(reinterpret_cast<const BYTE*>(internal_key_base64_decoded.data()));
    DataOut.cbData = static_cast<DWORD>(internal_key_base64_decoded.length());

    DATA_BLOB DataVerify;
    LPWSTR pDescrOut = NULL;
    
    string internal_key;

    if (CryptUnprotectData(
        &DataOut,
        &pDescrOut,
        NULL,                 // Optional entropy
        NULL,                 // Reserved
        NULL,                 // Here, the optional 
                            // prompt structure is not
                            // used.
        0,
        &DataVerify))
    {
        internal_key = string((char*)DataVerify.pbData, DataVerify.cbData);
    }
    else
    {
        printf("Internal key DPAPI decryption error!");
        return 1;
    }

    cout << internal_key.length() << endl;

    // read AES encrypted password
    fs.open("cred.csv");
    if (!fs.is_open())
    {
        cout << "Cannot open key file" << endl;
        return 1;
    }
    string encrypted_password;
    getline(fs, encrypted_password);
    fs.close();

    // go past comma
    encrypted_password = encrypted_password.substr(encrypted_password.find_first_of(",") + 1);
    // go past first "
    encrypted_password = encrypted_password.substr(encrypted_password.find_first_of("\"") + 1);
    // stop at next "
    encrypted_password = encrypted_password.substr(0, encrypted_password.find_first_of("\""));
    // remove v10
    encrypted_password = encrypted_password.substr(3);
    // get and remove 12-byte nonce
    string nonce = encrypted_password.substr(0, 12);
    encrypted_password = encrypted_password.substr(12);
    // remove last 16 bytes of magic number
    encrypted_password = encrypted_password.substr(0, encrypted_password.length() - 16);

    ofstream ofs("test.txt");
    ofs << internal_key << endl << nonce << endl << encrypted_password << endl;

}
