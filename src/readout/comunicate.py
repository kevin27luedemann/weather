import numpy as np
import serial as se
import time as ti
import datetime as dt
import sys, os
import influxdb as db
from optparse import OptionParser

def decode(msg):
    msg     = msg.split(' ')
    stamp   = dt.datetime.strptime(msg[0],"%d.%m.%YT%H:%M:%S")
    tem     = np.float(msg[1].split('C')[0])
    hum     = np.float(msg[1].split('C')[1][:-2])
    return (stamp,tem ,hum)

def main():
    parser = OptionParser()
    parser.add_option("-P", "--port", dest="port", default="/dev/ttyUSB0",
                      help="Specify which port to use, default ttyUSB0")
    parser.add_option("-s", action="store_true", dest="store", default=False,
                      help="store result in experiment database")
    parser.add_option("-t", "--time", dest="time", default="1.",
                      help="Specify the wait time, default 1s")
    (options, args) = parser.parse_args()
    ser = se.Serial(options.port, 9600)
    if options.store:
        client = db.InfluxDBClient(host='134.76.40.51', port=8086, database="experiment")
    cwd_measurement = os.getcwd().split("/")[3]
    while True:
        ser.write('t')
        msg = ser.readline()[:-1]
        sys.stdout.write(msg)
        sys.stdout.flush()
        stamp,tem,hum = decode(msg)
        db_output = {"measurement":cwd_measurement,
                    "time":dt.datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%SZ'),
                    "fields":{  "hdc1080_tem":tem,
                                "hdc1080_hum":hum}}
        if options.store:
            client.write_points([db_output])
        ti.sleep(np.float(options.time))

if __name__ == "__main__":
    main()
