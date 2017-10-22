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
    public partial class SVMCreate : Form
    {
        string folder_path = "";
        public delegate int do_lps_func(string file_name, int show_type);
        public SVMCreate()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "请选择文件路径";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                this.label1.Text = "请点击【生成】按钮";
                string foldPath = dialog.SelectedPath;
                folder_path = foldPath;
                this.textBox1.Text = foldPath;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.label1.Text = "正在处理中，请稍等......";
            Application.DoEvents();
            DllInvoke dll = new DllInvoke(@"../../../../x64/Release/CreateDLL.dll");
            do_lps_func lps = (do_lps_func)dll.Invoke("do_lps", typeof(do_lps_func));

            //folder_path = @"L:\Users\zc\Desktop\native_test";
            DirectoryInfo TheFolder = new DirectoryInfo(folder_path);

            int jpg_num = 0;

            //遍历文件获取jpg文件个数
            foreach (FileInfo NextFile in TheFolder.GetFiles())
            {
                if (NextFile.Extension == ".jpg")
                {
                    jpg_num++;
                }
            }

            //进度条初始化
            progressBar1.Value = 0;
            progressBar1.Minimum = 0;
            progressBar1.Maximum = jpg_num;

            //遍历文件
            foreach (FileInfo NextFile in TheFolder.GetFiles())
            {
                if (NextFile.Extension == ".jpg")
                {
                    progressBar1.Value++;
                    
                    int result_num = lps(folder_path + "\\" + NextFile.Name, 4);

                }
            }
            this.label1.Text = "文件夹内所有图片处理完毕，结果请点击下方按钮查看。";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string result_path = @"C:\Users\ZhangCan\Desktop\LPS\test_interface\bin\x64\Release\resources\dataset\svm";
            System.Diagnostics.Process.Start("explorer.exe", result_path);
        }

        private void SVMCreate_Load(object sender, EventArgs e)
        {

        }
    }
}
