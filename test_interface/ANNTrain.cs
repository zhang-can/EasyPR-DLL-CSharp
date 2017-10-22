using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace test_interface
{
    public partial class ANNTrain : Form
    {
        public ANNTrain()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.textBox1.AppendText("正在训练中，请稍等......\n");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("notepad.exe", "ann_result.txt");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string result_path = @"L:\Users\zc\Desktop\LPS\test_interface\bin\x64\Release\resources\train\ann";
            System.Diagnostics.Process.Start("explorer.exe", result_path);
        }
    }
}
