import serial
import time

ser = serial.Serial(
    port="/dev/ttyUSB0",
    baudrate=115200,
    timeout=1,
    xonxoff=False,
    rtscts=False,
    dsrdtr=False,
)


with open("PtpGmNtpServer.ucm", "r", newline="") as conf:

    for line in conf:
        if line.startswith("--"):
            continue

        ser.reset_input_buffer()

        ser.write(bytes(line, "ascii"))

        time.sleep(0.01)
        # Read everything available
        if ser.in_waiting > 0:
            buff = ser.read(ser.in_waiting).decode("ascii", errors="ignore")
        else:
            buff = ""

        for line in buff.splitlines():

            for prefix in ["$CR", "$WR", "$RR", "$ER"]:
                if line.startswith(prefix):
                    print(line)
                    break
