//import libraries
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#define MESSAGE_LENGTH 160

//The content of message sent
#define MESSAGE "Hello! Test Sim808 sms sending"

//Mobile phone number, need to change as the case might 
#define PHONE_NUMBER "+2349080885270"


char message[MESSAGE_LENGTH];
int messageIndex = 0;

char phone[13];
char datetime[24];

DFRobot_SIM808 sim808(&Serial);

void setup()
{
    Serial.begin(9600);
    
    //Initialize sim808 module
    while(!sim808.init())
    {
        Serial.print("Sim808 init error\r\n");
        delay(1000);
    }
    
    delay(3000);
    Serial.println("Init Success, continue");
    
    //Uncomment this after
    //send_sms();
    
    send_to_internet();
}

void loop()
{
    //read_sms();
    //Serial.println("Hello Arduino");
}

void send_to_internet()
{
    while(!sim808.connect(TCP, "127.0.0.1", 80))
    {
        Serial.println("Connection Error");
        
    }
    Serial.println("Connection To Internet Successful");

    char http_cmd[100];
    char buffer[512];
    
    sprintf(http_cmd, "GET /nafdac/index.php HTTP/1.0\r\n\r\n\0");
    sim808.send(http_cmd, strlen(http_cmd));
    
    //Send a GET request
    Serial.println("Waiting to fetch...");
    sim808.send(http_cmd, sizeof(http_cmd) - 1);
    
    while(true)
    {
        int ret = sim808.recv(buffer, sizeof(buffer) - 1);
        if(ret <= 0)
        {
            Serial.println("Fetch over...");
            break;
        }
        buffer[ret] = '\0';
        Serial.print("Recv: ");
        Serial.print(ret);
        Serial.print(" bytes: ");
        Serial.println(buffer);
        break;
    }
    
    //Close TCP or UCP connection
    sim808.close();
    
    //Disconnect wireless connection, Close Moving Scene
    sim808.disconnect();
    
}

void read_sms()
{
    /* Working as in reading unread sms */
    //Serial.println("Read Mock SMS");
    
    //Read Unread SMS
    messageIndex = sim808.isSMSunread();
    Serial.print("MessageIndex : ");
    Serial.println(messageIndex);
    
    //At least, there is one UNREAD SMS
    if(messageIndex > 0)
    {
        sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
        
        //In order not to full sim memory, is better to delete it
        sim808.deleteSMS(messageIndex);
        Serial.print("From Number : ");
        Serial.println(phone);
        Serial.println(datetime);
        Serial.print("Received Message: ");
        Serial.println(message);
    }
}


void send_sms()
{    //Working as in sending sms
    //Send sms to number
    sim808.sendSMS(PHONE_NUMBER, MESSAGE);
}
