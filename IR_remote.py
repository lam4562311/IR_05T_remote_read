#IRremote for Makefun and receiver-IR05T
import serial
import queue
from threading import *
import serial.tools.list_ports
import time

class IR_remote(serial.Serial):
    def __init__(self, port=None, description='USB2.0-Serial', baudrate=9600, VID='1A86', PID='7523'):
        ports = list(serial.tools.list_ports.comports())
        
        for p in ports:
            print(p.hwid, p.device, p.description)
            if f'VID:PID={VID}:{PID}' in p.hwid or description in p.description:
                device = p.device
                print("device:", device)
                break
        if port:
            super().__init__(port=port, baudrate=baudrate)
        else:
            super().__init__(port=device, baudrate=baudrate)
            
        self.button = {     '011111111222222221122111122112222345' : 1,
                            '011111111222222221112211122211222345' : 2,
                            '011111111222222221222212121111212345' : 3,
                            '011111111222222221112111122212222345' : 4,
                            '011111111222222222121211112121222345' : 'logo'}
        self.learn_init = bytearray([0xFD, 0xFD, 0xF1, 0xF2, 0xDF])
        self.result_queue = queue.Queue()
        # self.control_thread = Thread(target=self.LOOP_receive_val, args=[self.result_queue])
        self.control_thread = Thread(target=self.receive_from_MBED, args=[self.result_queue])
        self.control_thread.start()
        self.current_output = None
        
    def LOOP_receive_val(self, q):
        while True:
            # try:
                self.write(self.learn_init)
                learn_data = self.read_until(expected=b'\xDF\xDF') # use 'terminator' instead of 'expected' in old version of pyserial

                if learn_data[40] == 0x01:
                    valuable_data = self.button.get(learn_data[40:58].hex())
                    if valuable_data:
                        q.put(valuable_data)
                        print(q.queue[0])
                        print(q.qsize())
                    else:
                        print(learn_data[40:58].hex())
                    
                
                # if learn_data[]
            # except KeyboardInterrupt:
                # break
        self.close()
    def receive_from_mega(self, q):
        while True:
            
            learn_result = self.read_until(expected = b'\n').decode('ascii')
            print(learn_result)
            
            if learn_result == "invalid\n":
                pass
            if learn_result == "button_1\n":
                q.put(1)
            elif learn_result == "button_2\n":
                q.put(2)
            elif learn_result == "button_3\n":
                q.put(3)
            elif learn_result == "button_4\n":
                q.put(4)
            elif learn_result == "button_logo\n":
                q.put('logo')
    def receive_from_MBED(self, q):
        try: 
            while True:
                start = time.time()
                learn_result = self.read_until(expected = b'\n').decode('ascii')
                # print(learn_result, self.current_output, not (learn_result == self.current_output))
                end = time.time()
                # print (end - start)
                if not (learn_result == self.current_output):
                    self.current_output = learn_result
                # if end - start > 0.02:
                    if learn_result == "button_1\n":
                        q.put(1)
                    elif learn_result == "button_2\n":
                        q.put(2)
                    elif learn_result == "button_3\n":
                        q.put(3)
                    elif learn_result == "button_4\n":
                        q.put(4)
                    elif learn_result == "button_logo\n":
                        q.put('logo')
                
        except Exception as e:
            print(f"Remote Receiver ShutDown in Error {e}")
            self.close()
if __name__ == '__main__':
    # IRremote = IR_remote(VID='10C4', PID='EA60', baudrate = 9600)
    IRremote = IR_remote(description='STM32 STLink', VID='0483', PID='374B', baudrate = 9600)
    try:
        while True:

            if not IRremote.result_queue.empty():
                print(IRremote.result_queue.get())    
    except:
        IRremote.close()
    # ports = list(serial.tools.list_ports.comports())
    # for p in ports:
    #     print('www', p.hwid, p.device)
    #     # if 'VID:PID=10C4:EA60' in p.hwid:
    #     #     # Connection to port
    #     #     try:
    #     #         self.serial_obj = serial.Serial(p.device, baudrate, timeout = 0.5) # Linux