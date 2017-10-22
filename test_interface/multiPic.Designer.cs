namespace test_interface
{
    partial class multiPic
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.button2 = new System.Windows.Forms.Button();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Column5 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column3 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column4 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.文件ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.退出程序ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.模式切换ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.单图测试ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.批量测试ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.训练模型ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sVM训练ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aNN训练ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.生成样本ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sVM训练样本生成ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aNN训练样本ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.帮助ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.关于本系统ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(612, 50);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(95, 31);
            this.button1.TabIndex = 19;
            this.button1.Text = "开始";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(63, 50);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(111, 31);
            this.button2.TabIndex = 21;
            this.button2.Text = "选择文件夹";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.BackgroundColor = System.Drawing.SystemColors.ButtonHighlight;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Column5,
            this.Column1,
            this.Column2,
            this.Column3,
            this.Column4});
            this.dataGridView1.Location = new System.Drawing.Point(63, 123);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.RowHeadersVisible = false;
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(644, 391);
            this.dataGridView1.TabIndex = 22;
            // 
            // Column5
            // 
            this.Column5.HeaderText = "序号";
            this.Column5.Name = "Column5";
            this.Column5.ReadOnly = true;
            this.Column5.Width = 60;
            // 
            // Column1
            // 
            this.Column1.HeaderText = "正确车牌结果";
            this.Column1.Name = "Column1";
            this.Column1.ReadOnly = true;
            this.Column1.Width = 160;
            // 
            // Column2
            // 
            this.Column2.HeaderText = "检测车牌结果";
            this.Column2.Name = "Column2";
            this.Column2.ReadOnly = true;
            this.Column2.Width = 160;
            // 
            // Column3
            // 
            this.Column3.HeaderText = "结果差异（字符数）";
            this.Column3.Name = "Column3";
            this.Column3.ReadOnly = true;
            this.Column3.Width = 140;
            // 
            // Column4
            // 
            this.Column4.HeaderText = "用时（ms）";
            this.Column4.Name = "Column4";
            this.Column4.ReadOnly = true;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(63, 97);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(644, 11);
            this.progressBar1.TabIndex = 23;
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("微软雅黑", 12F);
            this.textBox1.Location = new System.Drawing.Point(172, 51);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(418, 29);
            this.textBox1.TabIndex = 24;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.label1.Location = new System.Drawing.Point(85, 45);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(126, 20);
            this.label1.TabIndex = 25;
            this.label1.Text = "平均用时（ms）：";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.label2.Location = new System.Drawing.Point(369, 45);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(107, 20);
            this.label2.TabIndex = 26;
            this.label2.Text = "完全正确比例：";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.label3.Location = new System.Drawing.Point(85, 82);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(97, 20);
            this.label3.TabIndex = 27;
            this.label3.Text = "误差≤1比例：";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.label4.Location = new System.Drawing.Point(369, 82);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(107, 20);
            this.label4.TabIndex = 28;
            this.label4.Text = "中文错误比例：";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.label5.Location = new System.Drawing.Point(252, 45);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(17, 20);
            this.label5.TabIndex = 29;
            this.label5.Text = "0";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.label6.Location = new System.Drawing.Point(520, 45);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(17, 20);
            this.label6.TabIndex = 30;
            this.label6.Text = "0";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.label7.Location = new System.Drawing.Point(252, 82);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(17, 20);
            this.label7.TabIndex = 31;
            this.label7.Text = "0";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("微软雅黑", 10F);
            this.label8.Location = new System.Drawing.Point(520, 82);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(17, 20);
            this.label8.TabIndex = 32;
            this.label8.Text = "0";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.groupBox1.Location = new System.Drawing.Point(63, 538);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(644, 125);
            this.groupBox1.TabIndex = 33;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "识别指标";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.文件ToolStripMenuItem,
            this.模式切换ToolStripMenuItem,
            this.训练模型ToolStripMenuItem,
            this.生成样本ToolStripMenuItem,
            this.帮助ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(791, 24);
            this.menuStrip1.TabIndex = 34;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 文件ToolStripMenuItem
            // 
            this.文件ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.退出程序ToolStripMenuItem});
            this.文件ToolStripMenuItem.Name = "文件ToolStripMenuItem";
            this.文件ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.文件ToolStripMenuItem.Text = "文件";
            // 
            // 退出程序ToolStripMenuItem
            // 
            this.退出程序ToolStripMenuItem.Name = "退出程序ToolStripMenuItem";
            this.退出程序ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.退出程序ToolStripMenuItem.Text = "退出程序";
            this.退出程序ToolStripMenuItem.Click += new System.EventHandler(this.退出程序ToolStripMenuItem_Click);
            // 
            // 模式切换ToolStripMenuItem
            // 
            this.模式切换ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.单图测试ToolStripMenuItem,
            this.批量测试ToolStripMenuItem});
            this.模式切换ToolStripMenuItem.Name = "模式切换ToolStripMenuItem";
            this.模式切换ToolStripMenuItem.Size = new System.Drawing.Size(67, 20);
            this.模式切换ToolStripMenuItem.Text = "模式切换";
            // 
            // 单图测试ToolStripMenuItem
            // 
            this.单图测试ToolStripMenuItem.Name = "单图测试ToolStripMenuItem";
            this.单图测试ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.单图测试ToolStripMenuItem.Text = "单图模式";
            this.单图测试ToolStripMenuItem.Click += new System.EventHandler(this.单图测试ToolStripMenuItem_Click_1);
            // 
            // 批量测试ToolStripMenuItem
            // 
            this.批量测试ToolStripMenuItem.Checked = true;
            this.批量测试ToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.批量测试ToolStripMenuItem.Name = "批量测试ToolStripMenuItem";
            this.批量测试ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.批量测试ToolStripMenuItem.Text = "批量模式";
            // 
            // 训练模型ToolStripMenuItem
            // 
            this.训练模型ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sVM训练ToolStripMenuItem,
            this.aNN训练ToolStripMenuItem});
            this.训练模型ToolStripMenuItem.Name = "训练模型ToolStripMenuItem";
            this.训练模型ToolStripMenuItem.Size = new System.Drawing.Size(67, 20);
            this.训练模型ToolStripMenuItem.Text = "训练模型";
            // 
            // sVM训练ToolStripMenuItem
            // 
            this.sVM训练ToolStripMenuItem.Name = "sVM训练ToolStripMenuItem";
            this.sVM训练ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.sVM训练ToolStripMenuItem.Text = "SVM训练";
            this.sVM训练ToolStripMenuItem.Click += new System.EventHandler(this.sVM训练ToolStripMenuItem_Click);
            // 
            // aNN训练ToolStripMenuItem
            // 
            this.aNN训练ToolStripMenuItem.Name = "aNN训练ToolStripMenuItem";
            this.aNN训练ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.aNN训练ToolStripMenuItem.Text = "ANN训练";
            this.aNN训练ToolStripMenuItem.Click += new System.EventHandler(this.aNN训练ToolStripMenuItem_Click);
            // 
            // 生成样本ToolStripMenuItem
            // 
            this.生成样本ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.sVM训练样本生成ToolStripMenuItem,
            this.aNN训练样本ToolStripMenuItem});
            this.生成样本ToolStripMenuItem.Name = "生成样本ToolStripMenuItem";
            this.生成样本ToolStripMenuItem.Size = new System.Drawing.Size(67, 20);
            this.生成样本ToolStripMenuItem.Text = "生成样本";
            // 
            // sVM训练样本生成ToolStripMenuItem
            // 
            this.sVM训练样本生成ToolStripMenuItem.Name = "sVM训练样本生成ToolStripMenuItem";
            this.sVM训练样本生成ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.sVM训练样本生成ToolStripMenuItem.Text = "SVM数据集";
            this.sVM训练样本生成ToolStripMenuItem.Click += new System.EventHandler(this.sVM训练样本生成ToolStripMenuItem_Click);
            // 
            // aNN训练样本ToolStripMenuItem
            // 
            this.aNN训练样本ToolStripMenuItem.Name = "aNN训练样本ToolStripMenuItem";
            this.aNN训练样本ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.aNN训练样本ToolStripMenuItem.Text = "ANN数据集";
            this.aNN训练样本ToolStripMenuItem.Click += new System.EventHandler(this.aNN训练样本ToolStripMenuItem_Click);
            // 
            // 帮助ToolStripMenuItem
            // 
            this.帮助ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.关于本系统ToolStripMenuItem});
            this.帮助ToolStripMenuItem.Name = "帮助ToolStripMenuItem";
            this.帮助ToolStripMenuItem.Size = new System.Drawing.Size(43, 20);
            this.帮助ToolStripMenuItem.Text = "帮助";
            // 
            // 关于本系统ToolStripMenuItem
            // 
            this.关于本系统ToolStripMenuItem.Name = "关于本系统ToolStripMenuItem";
            this.关于本系统ToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.关于本系统ToolStripMenuItem.Text = "关于本系统";
            // 
            // multiPic
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(791, 689);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox1);
            this.Name = "multiPic";
            this.Text = "批量模式 - 车牌字符分割系统";
            this.Load += new System.EventHandler(this.multiPic_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column5;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column2;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column3;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column4;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 文件ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 退出程序ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 模式切换ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 单图测试ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 批量测试ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 训练模型ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sVM训练ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aNN训练ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 生成样本ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sVM训练样本生成ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aNN训练样本ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 帮助ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 关于本系统ToolStripMenuItem;
    }
}