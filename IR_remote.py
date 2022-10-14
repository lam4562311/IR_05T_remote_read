#IRremote for Makefun and receiver-IR05T
import serial
import queue
from threading import *
import serial.tools.list_ports

class IR_remote(serial.Serial):
    def __init__(self, port=None, baudrate=9600, VID='10C4', PID='EA60'):
        ports = list(serial.tools.list_ports.comports())
        for p in ports:
            if f'VID:PID={VID}:{PID}' in p.hwid:
                device = p.device
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
        self.control_thread = Thread(target=self.LOOP_receive_val, args=[self.result_queue])
        self.control_thread.start()
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

if __name__ == '__main__':
    IRremote = IR_remote(VID='10C4', PID='EA60', baudrate = 9600)
    while True:
        if not IRremote.result_queue.empty():
            print(IRremote.result_queue.get())    
    IRremote.close()
    # ports = list(serial.tools.list_ports.comports())
    # for p in ports:
    #     print('www', p.hwid, p.device)
    #     # if 'VID:PID=10C4:EA60' in p.hwid:
    #     #     # Connection to port
    #     #     try:
    #     #         self.serial_obj = serial.Serial(p.device, baudrate, timeout = 0.5) # Linux