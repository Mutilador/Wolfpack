#################################################################
#   )      (\_     # WOLFPACK 13.0.0 Scripts                    #
#  ((    _/{  "-;  # Created by: DarkStorm                      #
#   )).-' {{ ;'`   # Revised by: MagnusBr                       #
#  ( (  ;._ \\ ctr # Last Modification: Created                 #
#################################################################

# Cotton
import wolfpack
import wolfpack.utilities
from wolfpack.consts import GRAY

ids = [ 0x10a4, 0x1015, 0x101c, 0x1019 ]
animids = [ 0x10a5, 0x1016, 0x101d, 0x101a ]

def onUse( char, item ):
	# Needs to be on ourself
	if item.getoutmostchar() != char:
		char.socket.clilocmessage( 500312, '', GRAY ) # You cannot reach that.
		return True

	char.socket.clilocmessage( 502655 ) # What spinning wheel do you wish to spin this on?
	char.socket.attachtarget( "cotton.response", [ item.serial ] )
	return True

def response( char, args, target ):
	direction = char.directionto( target.pos )
	if not char.direction == direction:
		char.direction = direction
		char.update()
	item = wolfpack.finditem( args[0] )

	if ( ( char.pos.x-target.pos.x )**2 + ( char.pos.y-target.pos.y )**2 > 4):
		char.socket.clilocmessage( 502648, '', GRAY) # Target is too far away.
		return True

	if abs( char.pos.z - target.pos.z ) > 5:
		char.socket.clilocmessage( 502648, '', GRAY) # Target is too far away.
		return True

	# Check target (only item targets valid)
	if not target.item:
		char.socket.clilocmessage( 502658, '', GRAY ) # Use that on a spinning wheel.
		return True

	if target.item.id in ids:
		color = item.color
		if ( item.amount > 1 ):
			item.amount = item.amount -1
			item.update()
		else:
			item.delete()

		# Spinning Wheel Animations
		if target.item.id == ids[0]:
			target.item.id = animids[0]
			target.item.update()
		elif target.item.id == ids[1]:
			target.item.id = animids[1]
			target.item.update()
		elif target.item.id == ids[2]:
			target.item.id = animids[2]
			target.item.update()
		elif target.item.id == ids[3]:
			target.item.id = animids[3]
			target.item.update()

		wheel = wolfpack.finditem( target.item.serial )
		processtime = 5000 # 5 Seconds
		char.addtimer( processtime, ProcessTimer, [wheel.serial, color] )

	elif target.item.id in animids:
		# That spinning wheel is being used.
		char.socket.clilocmessage( 502656, '', GRAY )
		return True

	else:
		# Use that on a spinning wheel.
		char.socket.clilocmessage( 502658, '', GRAY )
		return True

def ProcessTimer( char, args ):
	wheel = wolfpack.finditem(args[0])
	color = args[1]
	GetThread( char, wheel, color )
	return True

def GetThread( char, wheel, color ):
	# End the animations.
	if wheel.id == animids[0]:
		wheel.id = ids[0]
		wheel.update()
	elif wheel.id == animids[1]:
		wheel.id = ids[1]
		wheel.update()
	elif wheel.id == animids[2]:
		wheel.id = ids[2]
		wheel.update()
	elif wheel.id == animids[3]:
		wheel.id = ids[3]
		wheel.update()

	item_new = wolfpack.additem( 'fa0' ) # Spools of Thread
	item_new.amount = 6
	item_new.color = color
	if not wolfpack.utilities.tocontainer( item_new, char.getbackpack() ):
		item_new.update()

	if item_new.amount > 1:
		# You put the spools of thread in your backpack.
		char.socket.clilocmessage( 1010577, '', GRAY )
	else:
		# You put a spool of thread in your backpack.
		char.socket.clilocmessage( 1010575, '', GRAY )
	return True
