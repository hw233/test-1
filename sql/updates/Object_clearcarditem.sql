delete from item where id = 9981;
delete mail_package.* from mail_package,mail where mail_package.itemId=9981 and mail.mailId = mail_package.id;
