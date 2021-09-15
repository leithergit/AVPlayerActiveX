namespace SampleCS
{
    partial class FormPlayBack
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
            System.Windows.Forms.ListViewItem listViewItem10 = new System.Windows.Forms.ListViewItem("No.");
            System.Windows.Forms.ListViewItem listViewItem11 = new System.Windows.Forms.ListViewItem("StartTime");
            System.Windows.Forms.ListViewItem listViewItem12 = new System.Windows.Forms.ListViewItem("StopTime");
            this.listView1 = new System.Windows.Forms.ListView();
            this.listView2 = new System.Windows.Forms.ListView();
            this.Query = new System.Windows.Forms.Button();
            this.Play = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // listView1
            // 
            this.listView1.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem10,
            listViewItem11,
            listViewItem12});
            this.listView1.Location = new System.Drawing.Point(809, 211);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(230, 460);
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            // 
            // listView2
            // 
            this.listView2.Location = new System.Drawing.Point(809, 2);
            this.listView2.Name = "listView2";
            this.listView2.Size = new System.Drawing.Size(230, 163);
            this.listView2.TabIndex = 1;
            this.listView2.UseCompatibleStateImageBehavior = false;
            // 
            // Query
            // 
            this.Query.Location = new System.Drawing.Point(828, 171);
            this.Query.Name = "Query";
            this.Query.Size = new System.Drawing.Size(75, 34);
            this.Query.TabIndex = 2;
            this.Query.Text = "Query";
            this.Query.UseVisualStyleBackColor = true;
            this.Query.Click += new System.EventHandler(this.Query_Click);
            // 
            // Play
            // 
            this.Play.Location = new System.Drawing.Point(933, 171);
            this.Play.Name = "Play";
            this.Play.Size = new System.Drawing.Size(75, 34);
            this.Play.TabIndex = 2;
            this.Play.Text = "Play";
            this.Play.UseVisualStyleBackColor = true;
            this.Play.Click += new System.EventHandler(this.Play_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(1, 2);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(802, 669);
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // FormPlayBack
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1034, 672);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.Play);
            this.Controls.Add(this.Query);
            this.Controls.Add(this.listView2);
            this.Controls.Add(this.listView1);
            this.Name = "FormPlayBack";
            this.Text = "FormPlayBack";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ListView listView2;
        private System.Windows.Forms.Button Query;
        private System.Windows.Forms.Button Play;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}