import os
import paho.mqtt.client as mqtt
import mysql.connector
import logging

# Setup logging
logging.basicConfig(filename='iot.log', level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

# Koneksi ke database
try:
    mydb = mysql.connector.connect(
        host="localhost",
        user=os.getenv("DB_USER", "root"),  # Gunakan variabel environment jika tersedia
        password=os.getenv("DB_PASSWORD", ""),  # Gunakan variabel environment jika tersedia
        database="kotak_amal_pintar"
    )
    print("Koneksi ke database berhasil:", mydb)
except mysql.connector.Error as err:
    logging.error(f"Koneksi ke database gagal: {err}")
    exit(1)

mycursor = mydb.cursor()

# Callback saat berhasil terhubung ke broker MQTT
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected successfully with result code " + str(rc))
        client.subscribe("kotakamalpintar")
    else:
        print(f"Failed to connect, return code {rc}")
        logging.error(f"MQTT connection failed with return code {rc}")

# Callback saat pesan diterima
def on_message(client, userdata, msg):
    try:
        # Parsing data payload
        payload_str = str(msg.payload, 'utf-8')
        
        # Konversi payload ke float
        nominal = int(payload_str)
        print(f"Pesan diterima dari MQTT: {msg.topic} -> {msg.payload}")
        print(f'nominal : Rp.{nominal}')
        if(nominal > 0):
            sql = "INSERT INTO sedekah (nominal, timestamp) VALUES (%s, NOW())"
            val = (nominal,)  # Pastikan berbentuk tuple
            mycursor.execute(sql, val)
            mydb.commit()
            print("Sedekah diterima ke database.")
            print("TERIMAKASIH ORANG BAIK!!")

        logging.info(f"Data processed successfully: Nominal={nominal}")

    except ValueError as e:
        logging.error(f"Error parsing data: {e}")
        print(f"Error parsing data: {e}")
    except mysql.connector.Error as err:
        logging.error(f"Database error: {err}")
        print(f"Database error: {err}")
    except Exception as e:
        logging.error(f"Unexpected error: {e}")
        print(f"Unexpected error: {e}")

# Konfigurasi client MQTT
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

try:
    client.connect("mqtt.my.id", 1883, 60)
    logging.info("MQTT client connected to broker")
except Exception as e:
    logging.error(f"MQTT connection error: {e}")
    print(f"MQTT connection error: {e}")
    exit(1)

# Loop MQTT
client.loop_forever()