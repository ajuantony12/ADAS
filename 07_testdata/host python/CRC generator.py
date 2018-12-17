
SICK_STX = 0x02
SICK_ACK = 0x06
SICK_NAK = 0x15
SICK_NACK = 0x92
SICK_STATUS = 0x31
SICK_START = 0x02
SICK_DEST = 0x00 

def updateCRC(crc, ch, prev_ch):
    short_c = 0x00ff & ch
    short_p = (0x00ff & prev_ch) << 8
    if crc & 0x8000:
        crc = (crc << 1) ^ 0x8005
    else:
        crc = crc << 1
    crc &= 0xffff
    crc ^= (short_c | short_p)
    return crc
            
            

def calcSICK(string):
    crc = 0x0000
    prev_ch = 0
    for ch in string:
        crc = updateCRC(crc, ch, prev_ch)
        prev_ch = ch
    crc1 = crc & 0x00ff
    crc2 = crc >> 8
    return [crc1, crc2]

def create_and_send_msg(msg_to_send):
    msg = []
    msg.append(SICK_START)
    msg.append(SICK_DEST)
    length = len(msg_to_send)
    msg.append(length & 0x00ff) # split length to Hi and Lo byte
    msg.append(length >> 8)
    for i in msg_to_send:
        msg.append(i)
    crc = calcSICK(msg)
    msg.append(crc[0])
    msg.append(crc[1])
    print ( "Message: %s" % (''.join([hex(b)+" " for b in msg])))

msg_init = [0x10]
msg_ireset = [0x90]
msg_powerup = [0x91]
msg_request = [0x30, 0x00, 0x01]
msg_monitor_mode = [0x20, 0x21, 0x53, 0x49, 0x43, 0x4B, 0x5F, 0x50, 0x4C, 0x53]
msg_mode = [0x20, 0x00, 0x53, 0x49, 0x43, 0x4B, 0x5F, 0x50, 0x4C, 0x53]
msg_wf = [0x43, 0x0A, 0x18, 0x80, 0x19, 0x80, 0x1E, 0x80, 0x29, 0x80, 0x4E, 0x80, 0x4A, 0x80, 0x4E, 0x80, 0x5C, 0x80, 0x43, 0x80, 0x39, 0x80, 0x36, 0x80]


create_and_send_msg(msg_init)
create_and_send_msg(msg_ireset)
create_and_send_msg(msg_powerup)
create_and_send_msg(msg_request)
create_and_send_msg(msg_mode)
create_and_send_msg(msg_wf)
create_and_send_msg(msg_monitor_mode)