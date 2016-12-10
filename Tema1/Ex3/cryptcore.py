from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

class CryptCore:
    '''
    Purpose of this class is to help with encryption.
    CryptCore is build over cryptography class for this homework
    '''
    def __init__(self, iv):
        '''

        :param iv: initial vector for CFB mode
        '''
        self.iv = iv
        BS = 16
        self.pad = lambda s: s + (((BS - len(s)) % BS) * chr(BS - len(s) % BS)).encode()
        self.unpad = lambda s: s[:-ord(s[len(s) - 1:])] if ord(s[len(s) - 1:])<=16 else s
    def encrypt_blocks(self, mode_str: str, key, blocks):
        '''
        This is function that encrypt and return encrypted blocks in specified mode

        :param mode_str: mode of encryption ECB/CFB
        :param key: key to encrypt with the blocks
        :param blocks: list of blocks to encrypt
        :return: encrypted blocks
        '''
        mode = modes.ECB()
        cipher = Cipher(algorithms.AES(key), mode, backend=default_backend())
        if mode_str is "ECB":
            return self.encrypt_ECB(cipher, blocks)
        elif mode_str is "CFB":
            return self.encrypt_CFB(cipher, blocks)

    def encrypt_ECB(self, cipher, blocks):
        '''
        This is implementation of ECB mode

        :param cipher:  cipher object
        :param blocks: blocks to encrypt
        :return: encrypted blocks
        '''
        encrypted_blocks = []
        for block in blocks:
            encryptor = cipher.encryptor()
            ct = encryptor.update(block) + encryptor.finalize()
            encrypted_blocks.append(ct)
        return encrypted_blocks

    def encrypt_CFB(self, cipher, blocks):
        '''
        This is implementation of CFB mode

        :param cipher:  cipher object
        :param blocks: blocks to encrypt
        :return: encrypted blocks
        '''
        def xor(str1, str2):
            '''
            xor between two binary strings
            :param str1: string 1
            :param str2: string 2
            :return:
            '''
            result = bytes('', 'utf-8')
            for i in range(0, 16):
                xor = str1[i] ^ str2[i]
                # result of xor need to be in bytes hex format then i need to convert result of xor that is number.
                xor_x = xor.to_bytes((xor.bit_length() + 7) // 8, 'big')
                result += xor_x if len(xor_x) > 0 else b'\x00'
            return result

        encrypted_blocks = []
        encrypted_blocks.append(self.iv)
        for index in range(len(blocks)):
            encryptor = cipher.encryptor()
            Ek = encryptor.update(encrypted_blocks[index]) + encryptor.finalize()
            # print(blocks[index], ' xor ', Ek)
            Ci = xor(blocks[index], Ek)
            encrypted_blocks.append(Ci)
        return encrypted_blocks

    def decrypt_blocks(self, mode_str: str, key, blocks):
        '''
        This is decryption function it supports ECB and CFB modes

        :param mode_str: mode with which was encrypted blocks
        :param key: key with which was encrypted blocks
        :param blocks: list of encrypted blocks
        :return: lists of decrypted blocks
        '''
        mode = modes.ECB()
        cipher = Cipher(algorithms.AES(key), mode, backend=default_backend())
        if mode_str is "ECB":
            return self.decrypt_ECB(cipher, blocks)
        elif mode_str is "CFB":
            return self.decrypt_CFB(cipher, blocks)

    def decrypt_ECB(self, cipher, blocks):
        '''
        This function decrypts blocks with give cipher
        :param cipher: Cipher object to decrypt with
        :param blocks: list of blocks  to decrypt
        :return: decrypted blocks
        '''
        decrypted_blocks = []
        # print("user{user} decrypt".format(user=self.id))
        for block in blocks:
            decryptor = cipher.decryptor()
            dt = decryptor.update(block) + decryptor.finalize()
            decrypted_blocks.append(dt)
            s = decrypted_blocks[-1]
        decrypted_blocks[-1] = self.unpad(decrypted_blocks[-1])
        # print(decripted_blocks)
        return decrypted_blocks

    def decrypt_CFB(self, cipher, blocks):
        '''
        This function decrypts blocks with give cipher
        :param cipher: Cipher object to decrypt with
        :param blocks: list of blocks  to decrypt
        :return: decrypted blocks
        '''
        def xor(str1, str2):
            result = bytes('', 'utf-8')
            for i in range(0, 16):
                xor = str1[i] ^ str2[i]
                xor_x = xor.to_bytes((xor.bit_length() + 7) // 8, 'big')
                result += xor_x if len(xor_x) > 0 else '\x00'
            return result

        decrypted_blocks = []
        for index in range(1, len(blocks)):
            encryptor = cipher.encryptor()
            Ek = encryptor.update(blocks[index - 1]) + encryptor.finalize()
            pi = xor(blocks[index], Ek)
            decrypted_blocks.append(pi)
        decrypted_blocks[-1] = self.unpad(decrypted_blocks[-1])
        return decrypted_blocks
