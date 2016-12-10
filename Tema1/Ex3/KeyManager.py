import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

class KeyManager():

    def __init__(self, key_1, key_2, key_3):
        '''

        :param key_1: the key for ECB mode
        :param key_2: the key for CFB mode)
        :param key_3: the key_3 will be used to encrypt k1 or k2 the key_3 in known by all users ass well
        '''
        self.key_1 = key_1
        self.key_2 = key_2
        self.key_3 = key_3


    def get_key(self, mode):

        if mode is "ECB":
            # encrypt and send key1
            return self.encrypt_key(self.key_1)
        elif mode is "CFB":
            # encrypt and send key2
            return self.encrypt_key(self.key_2)
        return None


    def encrypt_key(self, key_to_send, mode = "ECB"):
        #encrypt and return key_to send with key_3 mode ECB

        cipher = Cipher(algorithms.AES(self.key_3), modes.ECB(), backend=default_backend())
        key_encryptor = cipher.encryptor()
        cripted_key = key_encryptor.update(key_to_send) + key_encryptor.finalize()
        return cripted_key

        pass
