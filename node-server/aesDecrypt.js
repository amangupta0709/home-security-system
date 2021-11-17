const CryptoJS = require("crypto-js")
const dotenv = require('dotenv');

dotenv.config();


// The AES encryption/decryption key to be used.
AESKey = process.env.AESKey
// AESKey = "2B7E151628AED2A6ABF7158809CF4F3C"

const decryption = (recieved) => {
    // recieved = "fuOwIDB7R6NggawR1RiaAg== : kXXJub1t7PJDWEG9id+RxQ=="
    //console.log(recieved)
    const data_arr = recieved.split(":");
    const esp8266_msg = data_arr[0].trim();
    const esp8266_iv = data_arr[1].trim();

    let plain_iv = new Buffer.from(esp8266_iv, 'base64').toString('hex');
    let iv = CryptoJS.enc.Hex.parse(plain_iv);
    let key = CryptoJS.enc.Hex.parse(AESKey);


    // Decrypt
    let bytes = CryptoJS.AES.decrypt(esp8266_msg, key, { iv: iv });
    let plaintext = bytes.toString(CryptoJS.enc.Base64);
    let decoded_b64msg = new Buffer.from(plaintext, 'base64').toString('ascii');
    let decoded_msg = new Buffer.from(decoded_b64msg, 'base64').toString('ascii');

    // console.log("Decryptedage: ", decoded_msg);
    let res;
    if(decoded_msg==process.env.PASSWORD){
        res = "Correct"
    }
    else {
        res = "Incorrect"
    }

    return res;
}

// const encryption = (data) = {

//     const iv = new Buffer.from('')   //(null) iv 
//     var password = 'a4e1112f45e84f785358bb86ba750f48';      //key password for cryptography

//     var cipher = CryptoJS.createCipheriv(algorithm,new Buffer(password),iv)

//      cipher = crypto.createCipheriv('aes-128-cbc',AESKey,iv);
//     var crypted = cipher.update(text,'utf8','base64');  //base64 , hex
//     crypted += cipher.final('base64');
//     return crypted;

//     console.log(encrypt(new Buffer('TextToEncrypt')).toString())
// }

module.exports = decryption