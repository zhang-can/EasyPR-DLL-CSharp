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
using System.Runtime.InteropServices;
using System.Threading;

namespace test_interface
{
    public partial class multiPic : Form
    {
        public multiPic()
        {
            InitializeComponent();
        }

        private void multiPic_Load(object sender, EventArgs e)
        {
            
        }

        string folder_path = "";

        float avg_time, zero_error_rate=0, one_error_rate=0, chinese_error_rate=0;

        public delegate int do_lps_func(string file_name, int show_type);

        public delegate IntPtr get_license_str_func();

        private void 模式切换ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            DllInvoke dll = new DllInvoke(@"../../../../x64/Release/CreateDLL.dll");
            do_lps_func lps = (do_lps_func)dll.Invoke("do_lps", typeof(do_lps_func));
            get_license_str_func get_license = (get_license_str_func)dll.Invoke("get_license_str", typeof(get_license_str_func));
            
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
                    int index = this.dataGridView1.Rows.Add();
                    dataGridView1.Rows[index].Cells[0].Value = index + 1;
                    dataGridView1.Rows[index].Cells[1].Value = NextFile.Name.Replace(".jpg", "");
                    System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();
                    watch.Start();//开始计时
                    int result_num = lps(folder_path + "\\" + NextFile.Name, 0);
                    watch.Stop();//结束计时
                    if (result_num == 0)
                    {
                        IntPtr license = get_license();
                        string license_str = Marshal.PtrToStringAnsi(license);
                        //第三列检测车牌结果
                        dataGridView1.Rows[index].Cells[2].Value = license_str;
                        //第四列误差
                        dataGridView1.Rows[index].Cells[3].Value = CompareText(license_str, NextFile.Name.Replace(".jpg", ""));
                        if(license_str[0] != NextFile.Name.Replace(".jpg", "")[0]){
                            chinese_error_rate++;
                        }
                        //有误行红色显示
                        if (CompareText(license_str, NextFile.Name.Replace(".jpg", "")) != 0)
                        {
                            dataGridView1.Rows[index].DefaultCellStyle.ForeColor = Color.Red;
                            if (CompareText(license_str, NextFile.Name.Replace(".jpg", "")) == 1)
                                one_error_rate++;
                        }
                        else
                        {
                            /*
                            //测试用，保存true
                            FileInfo file = new FileInfo(folder_path + "\\" + NextFile.Name);
                            if (file.Exists)
                            {
                                // true is overwrite
                                file.CopyTo(@"L:\Users\zc\Desktop\native_test\" + NextFile.Name, true);
                            }
                            */
                            zero_error_rate++;
                            one_error_rate++;
                        }
                        //第五列识别时间
                        dataGridView1.Rows[index].Cells[4].Value = watch.ElapsedMilliseconds;
                        if (index == 0)
                        {
                            avg_time = watch.ElapsedMilliseconds;
                        }
                        else
                        {
                            avg_time = (avg_time + watch.ElapsedMilliseconds) / 2;
                        }
                    }
                    
                }
            }
            this.label5.Text = avg_time.ToString();
            this.label6.Text = ((zero_error_rate / jpg_num)*100).ToString()+"%";
            this.label7.Text = ((one_error_rate / jpg_num)*100).ToString()+"%";
            this.label8.Text = ((chinese_error_rate / jpg_num) * 100).ToString() + "%";

        }

        private void button2_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "请选择文件路径";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                string foldPath = dialog.SelectedPath;
                folder_path = foldPath;
                this.textBox1.Text = foldPath;
            }
        }

        int CompareText(string str1, string str2)
        {
            int len = Math.Min(str1.Length, str2.Length);
            int error_num=0;
            for (int i = 0; i < len; i++)
            {
                if (str1[i] != str2[i])
                    error_num++;
            }
            return error_num;
        }

        private void 单图测试ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Hide();
            SinglePic f1 = new SinglePic();
            f1.ShowDialog();
            this.Close();
            
        }

        private void 单图测试ToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            this.Hide();
            SinglePic sglp = new SinglePic();
            sglp.ShowDialog();
            this.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {

        }

        private void 退出程序ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void sVM训练ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SVMTrain svm = new SVMTrain();
            svm.Show();
        }

        private void aNN训练ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ANNTrain ann = new ANNTrain();
            ann.Show();
        }

        private void sVM训练样本生成ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SVMCreate svmc = new SVMCreate();
            svmc.Show();
        }

        private void aNN训练样本ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ANNCreate annc = new ANNCreate();
            annc.Show();
        }
    }
}
