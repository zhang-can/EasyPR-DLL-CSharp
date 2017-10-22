using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace test_interface
{
    public partial class SVMTrain : Form
    {
        public SVMTrain()
        {
            InitializeComponent();
        }

        private void SVMTrain_Load(object sender, EventArgs e)
        {

        }

        public delegate int svm_train();

        private void button1_Click(object sender, EventArgs e)
        {
            this.textBox1.AppendText("正在训练中，请稍等......\n");
            
            Application.DoEvents();//C#界面假死导致label不更新解决
            DllInvoke dll = new DllInvoke(@"../../../../x64/Release/CreateDLL.dll");
            svm_train train_svm_model = (svm_train)dll.Invoke("train_svm", typeof(svm_train));
            
            if (train_svm_model() == 0)
            {
                this.textBox1.Text = "";
                this.textBox1.AppendText("模型训练完成！\n");
                this.textBox1.AppendText("\n");
                this.textBox1.AppendText("测试结果如下：\n");
                FileStream fs = new FileStream("svm_result.txt", FileMode.Open, FileAccess.ReadWrite);
                byte[] buff = new byte[1024];
                int len = fs.Read(buff, 0, 1024);
                while (len > 0)
                {
                    string s = Encoding.UTF8.GetString(buff, 0, len);
                    this.textBox1.AppendText(s);
                    len = fs.Read(buff, 0, 1024);
                }
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("notepad.exe", "svm_result.txt");
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            string result_path = @"L:\Users\zc\Desktop\LPS\test_interface\bin\x64\Release\resources\train\svm";
            System.Diagnostics.Process.Start("explorer.exe", result_path);
        }
    }
}
