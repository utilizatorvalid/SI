import os
from cryptcore import CryptCore

class User():
    '''
    This is user class
    Each user will have User{ID} folder in which he will store his files
    '''
    def __init__(self, id, key_manager, key_3, iv):
        '''

        :param id: user unique id
        :param key_manager: class get from key for file encryption
        :param key_3: key for decryption comunication key
        :param iv: initial vector used for CFB mode
        '''
        self.id = id
        self.key_manager = key_manager
        self.key_3 = key_3
        self.iv = iv
        self.users = {}
        self.path = os.path.join("User" + id)
        if not os.path.exists(self.path):
            os.mkdir("User" + id)
        self.crypt_helper = CryptCore(iv)

    def get_key(self, mode: str):
        '''
        This is function for used for getting key from key manager
        :param mode: mode communicate with
        :return: key for communication
        '''
        cripted_key = self.key_manager.get_key(mode)
        if cripted_key is None:
            raise Exception("unsupported mode to communicate", mode)

        return self.decrypt_key(cripted_key)

    def init_communication(self, other_user, mode):
        '''
        In this function
            1. A gets key for communication from keyManager
            2. A pings other_user
                2.1 other user gets key  from keyManager
                2.2 other user returns from ping True/False can/cant communicate
            3. saving data in users dictionary
        :param other_user:  user communication with
        :param mode: mode of communication ECB/CFB
        :return: True if can communicate (both users have the same key for communication) False otherwise
        '''
        print("User{a} Init communication User{a}(A) ----> User{b}(B)".format(a=self.id,
                                                                             b=other_user.id))
        key = self.get_key(mode)
        can_communicate = other_user.ping(mode, self)
        com_data = {}
        com_data['mode'] = mode
        com_data['key'] = key
        com_data['user'] = other_user
        com_data['can_communicate'] = can_communicate
        self.users[other_user.id] = com_data
        return can_communicate

    def ping(self, mode, ping_from):
        '''
        In this function:
            1. User gets key for communication from keyManager
            2. create com data object
            3. saving data in users dictionary
        :param mode: mode to communicate with
        :param ping_from: User from ping is sent
        :return: True if can communicate False otherwise
        '''
        print("User{b} Establishing connection between User{a} ----> User{b} ".format(a=ping_from.id, b=self.id))
        key = self.get_key(mode)
        com_data = {}
        com_data['mode'] = mode
        com_data['key'] = key
        com_data['user'] = ping_from
        com_data['can_communicate'] = True
        self.users[ping_from.id] = com_data
        return True

    def send_file(self, file_path: str, user):
        '''
        In this function we use:
            1. read_file function for getting blocks from file_paht relative to user sendbox
            2. encrypt_blocks from crypt_helper (class that I created in the same directory)
            3. call receive file from other_user for sending encrypted blocks
        :param file_path:  path to file to send
        :param user: user  send to
        :return: true if file is received False otherwise
        '''

        com_data = self.users.get(user.id)
        if com_data is None:
            print("Error unknown user to communicate with /init_comcunicatee or ping did not worked")
            return
        if not com_data['can_communicate']:
            print("Error user cant communicate yet")
            return
        blocks = self.read_file(file_path)
        # print(blocks)
        if len(blocks) < 1:
            print("EmptyFile")
            return 0
        # print(com_data['key'])
        encrypted_blocks = self.crypt_helper.encrypt_blocks(com_data['mode'], com_data['key'], blocks)
        other_user = com_data['user']
        if user is None:
            print("User send to not found", com_data)
            return
        print("send file : {file} {user_from}->{user_to}".format(file=file_path,
                                                                 user_from=self.path,
                                                                 user_to=other_user.path))

        return other_user.receive_file(file_path, encrypted_blocks, self.id)


    def receive_file(self, file_name, encrypted_blocks, received_from):
        '''

        :param file_name: file name for saving file
        :param encrypted_blocks: encrypted blocks
        :param received_from: user that send file
        :return: True if file is received False otherwise
        '''
        print("User{user} receive file:{file}".format(user=self.id,
                                                      file=file_name))
        com_data = self.users.get(received_from)
        if com_data is None:
            print("can't receive a file from unknown source :", received_from)

        decrypted_blocks = self.crypt_helper.decrypt_blocks(com_data['mode'], com_data['key'], encrypted_blocks)
        try:
            self.save_file(file_name, decrypted_blocks)
            return True
        except Exception as e:
            print("File is not send", e)
            return False
    def read_file(self, file):
        '''
        This is function that reads in block from user sendbox file
        :param file: reading file from user sendbox
        :return: block of size 128 bits
        '''
        blocks = []
        path = os.path.join(self.path, file)
        try:
            with open(path, 'rb') as f:
                while True:
                    block = f.read(16)
                    if block:
                        blocks.append(self.crypt_helper.pad(block))
                        # print("BEFORE")
                        # print(block, ':', len(block))
                        # print("AFTER")
                        # print(self.pad(block), ':', len(self.pad(block)))
                    else:
                        # print(blocks)
                        break

        except Exception as e:
            print("Exception", e)
        return blocks

    def save_file(self, file, blocks):
        '''

        :param file: file save to
        :param blocks:
        :return:
        '''
        path = os.path.join(self.path, file)
        try:
            with open(path, 'wb')as f:
                f.write(b''.join(blocks))
        except Exception as e:
            # print("Can't save file", self.id, e)
            raise Exception("Can't save file", self.id, e)

    def decrypt_key(self, cripted_key, mode="ECB"):
        key = self.crypt_helper.decrypt_blocks('ECB', self.key_3, [cripted_key])[0]
        return key
