using System.Net;
using System.Security.Cryptography;
using ThingsLibrary.Schema.Library;
using static System.Windows.Forms.LinkLabel;

namespace DexParser
{
    internal class DexDocument : ItemDto
    {
        public DexDocument()
        {
            
        }

        public static DexDocument Parse (string dexData)
        {
            var lines = dexData.Split("\n\r".ToCharArray(), StringSplitOptions.RemoveEmptyEntries);
                
            foreach(var line in lines)
            {
                var prefix2 = line.Substring(0, 2);
                var prefix3 = line.Substring(0, 3);

                //var handler = handlers[prefix3] || handlers[prefix2]

                //if (handler)
                //{
                //  handler(line, dexdata)
                //}            
            }

            
            //TODO:
            return new ();
        }

        private void Parse_ID1(string sentence)
        {
            var parts = sentence.Split('*');

            //    context.machine = context.machine || {}

            //    assign(parts, 1, context.machine, "serialNumber")
            //    assign(parts, 2, context.machine, "modelNumber")
            //    assign(parts, 3, context.machine, "buildStandard")
            //    assign(parts, 6, context.machine, "assetNumber")
        }

        private void Parse_CB1(string sentence)
        {
            //    var parts = line.split("*")
            //    context.machine = context.machine || {}
            //    context.machine.controlBoard = context.machine.controlBoard || { }

            //    assign(parts, 1, context.machine.controlBoard, "serialNumber")
            //    assign(parts, 2, context.machine.controlBoard, "modelNumber")
            //    assign(parts, 3, context.machine.controlBoard, "softwareRevision")
        }

        private void Parse_PA1(string sentence)
        {
        //    context.startItem()
        //    var parts = line.split("*")
        //    assign(parts, 1, context.currItem, "name")
        //    assign(parts, 2, context.currItem, "price", (i) => i / 100)
        }

        private void Parse_PA2(string sentence)
        {
            //var parsePA2 = function(line, context)
            //{
            //    context.ensureItem()
            //    var parts = line.split("*")
            //    assign(parts, 1, context.currItem, "sold", (i) => Number(i))
            //    assign(parts, 2, context.currItem, "revenue", (i) => i / 100)
        }

        private void Parse_PA3(string sentence)
        {
        //    context.ensureItem()
        //    var parts = line.split("*")
        //    assign(parts, 1, context.currItem, "testVendCount")
        }

        private void Parse_PA5(string sentence)
        {
        //    context.ensureItem()
        //    var parts = line.split("*")
        //    assign(parts, 1, context.currItem, "soldOutDate")
        //    assign(parts, 2, context.currItem, "soldOutTime")
        //    assign(parts, 3, context.currItem, "soldOutCount")
        }


        //var defaultHandlers =
        //{
        //    "ID1" : parseID1,
        //    "CB1" : parseCB1,
        //    "PA1" : parsePA1,
        //    "PA2" : parsePA2,
        //    "PA3" : parsePA3,
        //    "PA5" : parsePA5
        //};

        //CA1
        //CA10
        //CA15
        //CA16
        //CA2
        //CA3
        //CA4
        //CA8
        //CA9
        //CB1
        //DXE
        //DXS
        //EA2
        //EA3
        //EA7
        //G85
        //ID1
        //ID4
        //MA5
        //PA1
        //PA2
        //PA3
        //PA4
        //PA5
        //SD1
        //SE
        //ST
        //TA2
        //VA1
        //VA2
        //VA3

    }
}
