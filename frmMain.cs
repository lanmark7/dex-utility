using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DexParser
{
    public partial class frmMain : Form
    {
        public SerialPort? SerialDevice { get; set; }

        public frmMain()
        {
            InitializeComponent();

            //Set serial-port
            //this.SerialDevice = new SerialPort();
            //this.SerialDevice.PortName = portname;
            //this.SerialDevice.BaudRate = 9600;
            //this.SerialDevice.Parity = parity;
            //this.SerialDevice.DataBits = databits;
            //this.SerialDevice.StopBits = stopbits;
            //this.SerialDevice.DataReceived += new SerialDataReceivedEventHandler(SerialDevice_DataReceived);
            //this.SerialDevice.Open();
        }

        private void SerialDevice_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //SerialPort port = (SerialPort)sender;
            //_buffer = new byte[port.BytesToRead];
            //port.Read(_buffer, 0, _buffer.Length);

            ////Parse buffer
            //string sdata = "";
            //Encoding encoding = ASCIIEncoding.GetEncoding(1252);
            //if (null != _buffer)
            //{
            //    sdata = encoding.GetString(_buffer);
            //}
            //string[] string_array = sdata.Split('$');
            //string Gpgga = null;
            //for (int i = 0; i < string_array.Length; i++)
            //{
            //    string stringTemp = string_array[i];
            //    string[] line_array = stringTemp.Split(',');
            //    if (line_array[0] == "GPGGA")
            //    {
            //        Gpgga = string.Join(",", string_array[i]);
            //    }
            //}
            //this.LastGpgga = Gpgga;
        }            
    }
}
