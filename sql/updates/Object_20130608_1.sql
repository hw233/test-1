-- 强制阵旗，阵眼在交易区下架
update sale set saleTime=0 where itemId>1000 and itemId < 1200;
