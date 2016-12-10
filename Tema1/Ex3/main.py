import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

from KeyManager import KeyManager
from User import User

if __name__ == '__main__':
    '''
    This is an example of  working flow encrypted, sequentially communication between two or more Users
    features
        1. is supported bidirectional communication
        2. is supported communication with more than one user at the same time
        3. also encryption/decryption logic is apart from basic flow of communication
        4. next features:
            - communication User - User via server
            - for that only need to change mode of sending an receiving data (via KeyManager or other serverSide Service)
    '''
    ##########################
    # generating keys and iv #
    ##########################
    key_1 = os.urandom(16)
    key_2 = os.urandom(16)
    key_3 = os.urandom(16)
    iv = os.urandom(16)
    ##################################
    # creating keyManager an 3 users #
    ##################################
    km = KeyManager(key_1, key_2, key_3)
    u1 = User('1', km, key_3, iv)
    u2 = User('2', km, key_3, iv)
    u3 = User('3', km, key_3, iv)

    #    user 1 initiate communication with user2
    u1.init_communication(u2, "CFB")
    u1.send_file('file.txt', u2)

    u3.init_communication(u1,"ECB")
    u1.send_file('file.txt', u3)
    u2.send_file('file2.txt',u1)
