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
    public partial class DetectShow : Form
    {
        public DetectShow()
        {
            InitializeComponent();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void DetectShow_Load(object sender, EventArgs e)
        {
            //MSER-
            //1
            if (File.Exists(@"resources/image/interface/show_detect/0.png"))
            {
                FileStream pFileStream1 = new FileStream(@"resources/image/interface/show_detect/0.png", FileMode.Open, FileAccess.Read);
                pictureBox1.Image = Image.FromStream(pFileStream1);  //动态添加图片 
                pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream1.Close();
                pFileStream1.Dispose();
            }
            
            //2
            if (File.Exists(@"resources/image/interface/show_detect/1.png"))
            {
                FileStream pFileStream2 = new FileStream(@"resources/image/interface/show_detect/1.png", FileMode.Open, FileAccess.Read);
                pictureBox2.Image = Image.FromStream(pFileStream2);  //动态添加图片 
                pictureBox2.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream2.Close();
                pFileStream2.Dispose();
            }
            
            //3
            if (File.Exists(@"resources/image/interface/show_detect/image_charVec_BLUE.png"))
            {
                FileStream pFileStream3 = new FileStream(@"resources/image/interface/show_detect/image_charVec_BLUE.png", FileMode.Open, FileAccess.Read);
                pictureBox3.Image = Image.FromStream(pFileStream3);  //动态添加图片 
                pictureBox3.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream3.Close();
                pFileStream3.Dispose();
            }

            //4
            if (File.Exists(@"resources/image/interface/show_detect/charVec_remove_overlap_BLUE.png"))
            {
                FileStream pFileStream4 = new FileStream(@"resources/image/interface/show_detect/charVec_remove_overlap_BLUE.png", FileMode.Open, FileAccess.Read);
                pictureBox4.Image = Image.FromStream(pFileStream4);  //动态添加图片 
                pictureBox4.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream4.Close();
                pFileStream4.Dispose();
            }

            //5
            if (File.Exists(@"resources/image/interface/show_detect/result_strong_seed_BLUE.png"))
            {
                FileStream pFileStream5 = new FileStream(@"resources/image/interface/show_detect/result_strong_seed_BLUE.png", FileMode.Open, FileAccess.Read);
                pictureBox8.Image = Image.FromStream(pFileStream5);  //动态添加图片 
                pictureBox8.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream5.Close();
                pFileStream5.Dispose();
            }

            //6
            if (File.Exists(@"resources/image/interface/show_detect/white_line_BLUE.png"))
            {
                FileStream pFileStream6 = new FileStream(@"resources/image/interface/show_detect/white_line_BLUE.png", FileMode.Open, FileAccess.Read);
                pictureBox7.Image = Image.FromStream(pFileStream6);  //动态添加图片 
                pictureBox7.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream6.Close();
                pFileStream6.Dispose();
            }

            //7
            if (File.Exists(@"resources/image/interface/show_detect/all_searched_BLUE.png"))
            {
                FileStream pFileStream7 = new FileStream(@"resources/image/interface/show_detect/all_searched_BLUE.png", FileMode.Open, FileAccess.Read);
                pictureBox6.Image = Image.FromStream(pFileStream7);  //动态添加图片 
                pictureBox6.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream7.Close();
                pFileStream7.Dispose();
            }

            //8
            if (File.Exists(@"resources/image/interface/show_detect/mser_result_BLUE.png"))
            {
                FileStream pFileStream8 = new FileStream(@"resources/image/interface/show_detect/mser_result_BLUE.png", FileMode.Open, FileAccess.Read);
                pictureBox5.Image = Image.FromStream(pFileStream8);  //动态添加图片 
                pictureBox5.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream8.Close();
                pFileStream8.Dispose();
            }


            //MSER+
            //1
            if (File.Exists(@"resources/image/interface/show_detect/2.png"))
            {
                FileStream pFileStream1 = new FileStream(@"resources/image/interface/show_detect/2.png", FileMode.Open, FileAccess.Read);
                pictureBox16.Image = Image.FromStream(pFileStream1);  //动态添加图片 
                pictureBox16.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream1.Close();
                pFileStream1.Dispose();
            }

            //2
            if (File.Exists(@"resources/image/interface/show_detect/3.png"))
            {
                FileStream pFileStream2 = new FileStream(@"resources/image/interface/show_detect/3.png", FileMode.Open, FileAccess.Read);
                pictureBox15.Image = Image.FromStream(pFileStream2);  //动态添加图片 
                pictureBox15.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream2.Close();
                pFileStream2.Dispose();
            }

            //3
            if (File.Exists(@"resources/image/interface/show_detect/image_charVec_YELLOW.png"))
            {
                FileStream pFileStream3 = new FileStream(@"resources/image/interface/show_detect/image_charVec_YELLOW.png", FileMode.Open, FileAccess.Read);
                pictureBox13.Image = Image.FromStream(pFileStream3);  //动态添加图片 
                pictureBox13.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream3.Close();
                pFileStream3.Dispose();
            }

            //4
            if (File.Exists(@"resources/image/interface/show_detect/charVec_remove_overlap_YELLOW.png"))
            {
                FileStream pFileStream4 = new FileStream(@"resources/image/interface/show_detect/charVec_remove_overlap_YELLOW.png", FileMode.Open, FileAccess.Read);
                pictureBox10.Image = Image.FromStream(pFileStream4);  //动态添加图片 
                pictureBox10.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream4.Close();
                pFileStream4.Dispose();
            }

            //5
            if (File.Exists(@"resources/image/interface/show_detect/result_strong_seed_YELLOW.png"))
            {
                FileStream pFileStream5 = new FileStream(@"resources/image/interface/show_detect/result_strong_seed_YELLOW.png", FileMode.Open, FileAccess.Read);
                pictureBox14.Image = Image.FromStream(pFileStream5);  //动态添加图片 
                pictureBox14.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream5.Close();
                pFileStream5.Dispose();
            }

            //6
            if (File.Exists(@"resources/image/interface/show_detect/white_line_YELLOW.png"))
            {
                FileStream pFileStream6 = new FileStream(@"resources/image/interface/show_detect/white_line_YELLOW.png", FileMode.Open, FileAccess.Read);
                pictureBox12.Image = Image.FromStream(pFileStream6);  //动态添加图片 
                pictureBox12.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream6.Close();
                pFileStream6.Dispose();
            }

            //7
            if (File.Exists(@"resources/image/interface/show_detect/all_searched_YELLOW.png"))
            {
                FileStream pFileStream7 = new FileStream(@"resources/image/interface/show_detect/all_searched_YELLOW.png", FileMode.Open, FileAccess.Read);
                pictureBox11.Image = Image.FromStream(pFileStream7);  //动态添加图片 
                pictureBox11.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream7.Close();
                pFileStream7.Dispose();
            }

            //8
            if (File.Exists(@"resources/image/interface/show_detect/mser_result_YELLOW.png"))
            {
                FileStream pFileStream8 = new FileStream(@"resources/image/interface/show_detect/mser_result_YELLOW.png", FileMode.Open, FileAccess.Read);
                pictureBox9.Image = Image.FromStream(pFileStream8);  //动态添加图片 
                pictureBox9.SizeMode = PictureBoxSizeMode.StretchImage;  //使控件PictureBox的大小适应图片的大小
                pFileStream8.Close();
                pFileStream8.Dispose();
            }
        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click_1(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\0.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\1.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\image_charVec_BLUE.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\charVec_remove_overlap_BLUE.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox8_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\result_strong_seed_BLUE.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox7_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\white_line_BLUE.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox6_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\all_searched_BLUE.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\mser_result_BLUE.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox16_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\2.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox15_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\3.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox13_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\image_charVec_YELLOW.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox10_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\charVec_remove_overlap_YELLOW.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox14_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\result_strong_seed_YELLOW.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox12_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\white_line_YELLOW.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox11_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\all_searched_YELLOW.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox9_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_detect\mser_result_YELLOW.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }
    }
}
