import random
import sys

while(1):
	execute = input("input number (execute : 1, exit : 0) :")
	if(execute == '0'):
		sys.exit("exit")
	elif(execute != '1'):
		continue
	gacha_num = random.randrange(0,10)
	print("뽑은 숫자 : ",gacha_num)
	if(gacha_num == 0):
		print("1등 당첨 (확률 : 10%)")
		continue
	elif(gacha_num > 5):
		print("4등 당첨 (확률 : 40%)")
		continue
	elif(gacha_num > 2):
		print("3등 당첨 (확률 : 30%)")
		continue
	elif(gacha_num > 0):
		print("2등 당첨 (확률 : 20%)")
		continue
