#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
import sys


def turtle_circle(speed,ang):
	rospy.init_node('turtlesim', anonymous=True)
	pub = rospy.Publisher('/turtle1/cmd_vel',
						Twist, queue_size=10)
	rate = rospy.Rate(10)
	vel = Twist()
	option = 0
	while not rospy.is_shutdown():
		while(option == 0):
			if(speed >= 15):
				speed = speed * -1
				ang = ang * -1
				option = 1
				#print("option = 1")
				break
			rospy.loginfo("speed = %f, ang = %f", speed, ang)
			vel.linear.x = speed
			vel.linear.y = 0
			vel.linear.z = 0

			vel.angular.x = 0
			vel.angular.y = 0
			vel.angular.z = ang
			speed = speed + 0.005
			pub.publish(vel)
			rate.sleep()
		while(option > 0):
			#print("option > 0")
			rospy.loginfo("speed = %f, ang = %f", speed, ang)
                        vel.linear.x = speed
                        vel.linear.y = 0
                        vel.linear.z = 0

                        vel.angular.x = 0
                        vel.angular.y = 0
                        vel.angular.z = ang
                        speed = speed + 0.005
                        pub.publish(vel)
                        rate.sleep()
			if(speed >= 0):
				sys.exit()

if __name__ == '__main__':
	try:
		turtle_circle(float(sys.argv[1]),float(sys.argv[2]))
	except rospy.ROSInterruptException:
		pass

