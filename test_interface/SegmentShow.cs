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
    public partial class SegmentShow : Form
    {
        public SegmentShow()
        {
            InitializeComponent();
        }

        private void SegmentShow_Load(object sender, EventArgs e)
        {
            if (File.Exists(@"resources/image/interface/show_segment/0.png"))
            {
                FileStream pFileStream0 = new FileStream(@"resources/image/interface/show_segment/0.png", FileMode.Open, FileAccess.Read);
                pictureBox1.Image = Image.FromStream(pFileStream0);
                pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
                pFileStream0.Close();
                pFileStream0.Dispose();
            }

            FileStream pFileStream1 = new FileStream(@"resources/image/interface/show_segment/1.png", FileMode.Open, FileAccess.Read);
            pictureBox2.Image = Image.FromStream(pFileStream1);
            pictureBox2.SizeMode = PictureBoxSizeMode.Zoom;
            pFileStream1.Close();
            pFileStream1.Dispose();

            FileStream pFileStream2 = new FileStream(@"resources/image/interface/show_segment/2.png", FileMode.Open, FileAccess.Read);
            pictureBox8.Image = Image.FromStream(pFileStream2);
            pictureBox8.SizeMode = PictureBoxSizeMode.Zoom;
            pFileStream2.Close();
            pFileStream2.Dispose();

            FileStream pFileStream3 = new FileStream(@"resources/image/interface/show_segment/3.png", FileMode.Open, FileAccess.Read);
            pictureBox4.Image = Image.FromStream(pFileStream3);
            pictureBox4.SizeMode = PictureBoxSizeMode.Zoom;
            pFileStream3.Close();
            pFileStream3.Dispose();

            FileStream pFileStream4 = new FileStream(@"resources/image/interface/show_segment/4.png", FileMode.Open, FileAccess.Read);
            pictureBox3.Image = Image.FromStream(pFileStream4);
            pictureBox3.SizeMode = PictureBoxSizeMode.Zoom;
            pFileStream4.Close();
            pFileStream4.Dispose();

            FileStream pFileStream5 = new FileStream(@"resources/image/interface/show_segment/5.png", FileMode.Open, FileAccess.Read);
            pictureBox5.Image = Image.FromStream(pFileStream5);
            pictureBox5.SizeMode = PictureBoxSizeMode.Zoom;
            pFileStream5.Close();
            pFileStream5.Dispose();

            FileStream pFileStream6 = new FileStream(@"resources/image/interface/show_segment/6.png", FileMode.Open, FileAccess.Read);
            pictureBox7.Image = Image.FromStream(pFileStream6);
            pictureBox7.SizeMode = PictureBoxSizeMode.Zoom;
            pFileStream6.Close();
            pFileStream6.Dispose();

            FileStream pFileStream7 = new FileStream(@"resources/image/interface/show_segment/7.png", FileMode.Open, FileAccess.Read);
            pictureBox6.Image = Image.FromStream(pFileStream7);
            pictureBox6.SizeMode = PictureBoxSizeMode.Zoom;
            pFileStream7.Close();
            pFileStream7.Dispose();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_segment\0.png";      
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";    
            process.StartInfo.UseShellExecute = true;    
            process.Start();
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_segment\1.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox8_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_segment\2.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_segment\3.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_segment\4.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_segment\5.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox7_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_segment\6.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }

        private void pictureBox6_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process process = new System.Diagnostics.Process();
            process.StartInfo.FileName = @"resources\image\interface\show_segment\7.png";
            process.StartInfo.Arguments = "rundll32.exe L://WINDOWS//system32//shimgvw.dll";
            process.StartInfo.UseShellExecute = true;
            process.Start();
        }
    }
}
