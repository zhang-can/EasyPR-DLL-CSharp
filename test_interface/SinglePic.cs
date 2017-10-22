using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace test_interface
{
    

    public partial class SinglePic : Form
    {

        public SinglePic()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }

        public delegate int test02_func(int a, int b);

        public delegate int do_lps_func(string file_name, int show_type);

        public delegate IntPtr get_license_str_func();

        public delegate IntPtr get_color_str_func();

        private void button1_Click(object sender, EventArgs e)
        {
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void 打开ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DllInvoke dll = new DllInvoke(@"../../../../x64/Release/CreateDLL.dll");
            do_lps_func lps = (do_lps_func)dll.Invoke("do_lps", typeof(do_lps_func));
            get_license_str_func get_license = (get_license_str_func)dll.Invoke("get_license_str", typeof(get_license_str_func));
            get_color_str_func get_color = (get_color_str_func)dll.Invoke("get_color_str", typeof(get_color_str_func));

            OpenFileDialog fileDialog = new OpenFileDialog();
            fileDialog.Title = "请选择文件";
            fileDialog.Filter = "所有文件(*.*)|*.*";
            if (fileDialog.ShowDialog() == DialogResult.OK)
            {
                Directory.Delete(@"resources/image/interface/show_detect", true);
                Directory.CreateDirectory(@"resources/image/interface/show_detect");
                string file_name = fileDialog.FileName;
                //MessageBox.Show("已选择文件:" + file_name, "选择文件提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                int result_num = lps(file_name, 2);// 只写结果
                if (result_num == 0)
                {
                    IntPtr license = get_license();
                    string license_str = Marshal.PtrToStringAnsi(license);
                    IntPtr color = get_color();
                    string color_str = Marshal.PtrToStringAnsi(color);
                    string color_CHN;
                    if (color_str == "BLUE")
                        color_CHN = "蓝牌：";
                    else if (color_str == "YELLOW")
                        color_CHN = "黄牌：";
                    else if (color_str == "GREEN")
                        color_CHN = "绿牌：";
                    else
                        color_CHN = "未知：";

                    this.textBox1.Text = color_CHN + license_str;

                    FileStream pFileStream0 = new FileStream(@"resources/image/interface/chars_segment/whole.png", FileMode.Open, FileAccess.Read);
                    pictureBox1.Image = Image.FromStream(pFileStream0);
                    pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
                    pFileStream0.Close();
                    pFileStream0.Dispose();

                    //pictureBox1.Image = Image.FromFile(@"resources/image/interface/chars_segment/whole.png");  //动态添加图片  
                    //pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;  //使控件PictureBox的大小适应图片的大小

                    FileStream pFileStream1 = new FileStream(@"resources/image/interface/final_result/plate_0.png", FileMode.Open, FileAccess.Read);
                    pictureBox2.Image = Image.FromStream(pFileStream1);  //动态添加图片 
                    pictureBox2.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                    pFileStream1.Close();
                    pFileStream1.Dispose();

                    FileStream pFileStream2 = new FileStream(@"resources/image/interface/chars_segment/0.png", FileMode.Open, FileAccess.Read);
                    char0.Image = Image.FromStream(pFileStream2);  //动态添加图片 
                    char0.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                    pFileStream2.Close();
                    pFileStream2.Dispose();

                    FileStream pFileStream3 = new FileStream(@"resources/image/interface/chars_segment/1.png", FileMode.Open, FileAccess.Read);
                    char1.Image = Image.FromStream(pFileStream3);  //动态添加图片 
                    char1.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                    pFileStream3.Close();
                    pFileStream3.Dispose();

                    FileStream pFileStream4 = new FileStream(@"resources/image/interface/chars_segment/2.png", FileMode.Open, FileAccess.Read);
                    char2.Image = Image.FromStream(pFileStream4);  //动态添加图片 
                    char2.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                    pFileStream4.Close();
                    pFileStream4.Dispose();

                    FileStream pFileStream5 = new FileStream(@"resources/image/interface/chars_segment/3.png", FileMode.Open, FileAccess.Read);
                    char3.Image = Image.FromStream(pFileStream5);  //动态添加图片 
                    char3.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                    pFileStream5.Close();
                    pFileStream5.Dispose();

                    FileStream pFileStream6 = new FileStream(@"resources/image/interface/chars_segment/4.png", FileMode.Open, FileAccess.Read);
                    char4.Image = Image.FromStream(pFileStream6);  //动态添加图片 
                    char4.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                    pFileStream6.Close();
                    pFileStream6.Dispose();

                    FileStream pFileStream7 = new FileStream(@"resources/image/interface/chars_segment/5.png", FileMode.Open, FileAccess.Read);
                    char5.Image = Image.FromStream(pFileStream7);  //动态添加图片 
                    char5.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                    pFileStream7.Close();
                    pFileStream7.Dispose();

                    FileStream pFileStream8 = new FileStream(@"resources/image/interface/chars_segment/6.png", FileMode.Open, FileAccess.Read);
                    char6.Image = Image.FromStream(pFileStream8);  //动态添加图片 
                    char6.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                    pFileStream8.Close();
                    pFileStream8.Dispose();

                    FileStream pFileStream9 = new FileStream(@"resources/image/interface/chars_segment/7.png", FileMode.Open, FileAccess.Read);
                    if (color_str == "GREEN")
                    {
                        char7.Visible = true;
                    }
                    else
                    {
                        char7.Visible = false;
                    }
                        
                    char7.Image = Image.FromStream(pFileStream9);  //动态添加图片 
                    char7.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                    pFileStream9.Close();
                    pFileStream9.Dispose();

                }
            }
        }

        private void 批量测试ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Hide();
            multiPic mp = new multiPic();
            mp.ShowDialog();
            this.Close();
        }

        private void 字符分割ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /*DllInvoke dll = new DllInvoke(@"../../../../x64/Release/CreateDLL.dll");
            test02_func test2 = (test02_func)dll.Invoke("test02", typeof(test02_func));
            int r2 = test2(5, 2);
            this.label1.Text = r2.ToString();*/
            SegmentShow segshow = new SegmentShow();
            segshow.Show();
        }

        private void 退出程序ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void 分步演示ToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void 车牌定位ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DetectShow ds = new DetectShow();
            ds.Show();
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

        private void aNN训练样本ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ANNCreate annc = new ANNCreate();
            annc.Show();
        }

        private void sVM训练样本生成ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SVMCreate svmc = new SVMCreate();
            svmc.Show();
        }

    }
}
