# ECE6612_ChromePassword
## Steps (Only works on Windows)
- Obtain your encrypted key from the ```os_crypt.encrypted_key``` field in C:\Users\<your_username>\AppData\Local\Google\Chrome\User Data\Local State
- Copy the encrypted key into a file called "crack_chrome.txt" in the project directory (an example is provided, but it wouldn't work on your computer)
- Obtain your local username and password database from C:\Users\<your_username>\AppData\Local\Google\Chrome\User Data\Default\Login Data
- Use SQLite to export the usernames and passwords into a .csv file
```
sqlite> .mode csv
sqlite> .output cred.csv
sqlite> select username_value,password_value from logins;
sqlite> .output stdout
```
- Copy the .csv file to the project directory
- Run the console program, which will decrypt the encryption key and pre-process the first encrypted password in the .csv file
  - This step will generate a test.txt file which is used in the next step
- Run the Jupyter Notebook Python script testdec.ipynb
  - The plaintext password should show up in the final step
## Example Output
Here is a password on the Google Chrome Autofill page and on the decrypted password using the steps above
----------------------------------------------------------------
![Chrome](/demo_image/chrome_autofill_page.JPG)
----------------------------------------------------------------
![Python](/demo_image/final_step_of_python_script.JPG)
