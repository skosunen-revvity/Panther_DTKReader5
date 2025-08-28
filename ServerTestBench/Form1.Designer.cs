namespace ServerTestBench {
    partial class Form1 {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.cmdInitialize = new System.Windows.Forms.Button();
            this.lblImage = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // cmdInitialize
            // 
            this.cmdInitialize.Location = new System.Drawing.Point(16, 16);
            this.cmdInitialize.Name = "cmdInitialize";
            this.cmdInitialize.Size = new System.Drawing.Size(75, 23);
            this.cmdInitialize.TabIndex = 0;
            this.cmdInitialize.Text = "Initialize";
            this.cmdInitialize.UseVisualStyleBackColor = true;
            this.cmdInitialize.Click += new System.EventHandler(this.cmdInitialize_Click);
            // 
            // lblImage
            // 
            this.lblImage.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblImage.Location = new System.Drawing.Point(16, 48);
            this.lblImage.Name = "lblImage";
            this.lblImage.Size = new System.Drawing.Size(752, 480);
            this.lblImage.TabIndex = 1;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(834, 665);
            this.Controls.Add(this.lblImage);
            this.Controls.Add(this.cmdInitialize);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button cmdInitialize;
        private System.Windows.Forms.Label lblImage;
    }
}

