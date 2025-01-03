-- Commands to creat cutscene

	-- To add serial and parallel commands
	-- AddSerialCommand(friendlyName (string) , Command (string), (number)...(number)) - As many as necessary for the command but the last one should be time
	-- AddParallelCommand(friendlyName (string), Command (string), (number)...(number)) - As many as necessary for the command but the last one should be time
		
		-- Commands:
			-- Move Relative Linear
			-- Move Relative EaseInAndOut: (friendly name, command, x, y, z, time)
			-- OrientTo: (friendly name, x, y, z, time)
			-- Follow Curve CubicBezier: (friendly name, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, time) -- Includes 4 control points
			-- Follow Linear Facing: (follower, command, followed, maxSpeed, maxSpeedDistance, minDistance,  xOffSet, yOffSet, zOffSet, time ) -- Includes 4 control points
			-- AddVelocity: objectToAddVelocity, directionX, directionY, directionZ, timeToStartMove, velocity, timeMoving
			-- Wait Time: friendly name, command, time

	-- To add location triggers
	-- AddLocationTriggerCommand(trigger friendlyName, commanded friendlyName(string), (number)...(number))

		-- Commands:
			-- Follow Triggered Linear: (triggerer, commanded, command, x, y, z, radius, time, followDistance,offSetX, offSetY, offSetZ, maxFollowSpeed, minAccRange, maxAccRange )
				-- e.g - AddLocationTriggerCommand("camera", "beholder", "Follow Triggered Linear", (number)...(number))

	-- To create command groups
		-- CreateCommandGroup()
			-- AddSerialCommand()
			-- AddParallelCommand()
		-- CompleteCommandGroup()

	-- To add animations that will loop forever
	-- AddLoopingAnimation

		-- Commands:
			-- FlickerLight:  (light name, atten1X, atten1Y, atten1Z, atten1W, atten2X, atten2Y, atten2Z, atten2W,flicker speed)

AddLoopingAnimation("torch1", "FlickerLight", 
0.0, 0.001, 0.003, 0.001, -- attenuation 1 
0.0, 0.001, 0.005, 0.0008, -- attenuation 2
5.0) -- flicker speed

AddLocationTriggerCommand("camera", "beholder", "Follow Triggered Linear",
	1000.0, 0.0, 1000.0, -- centerPosition
	800.0,  -- radius
	8.0, -- time
	200.0, -- followDistance
	0.0, 0.0, 0.0, -- offset
	300.0, -- maxFollowSpeed 430
	500.0, 700.0) -- minAccRange, maxAccRange

CreateCommandGroup()
AddSerialCommand("camera", "Move Relative EaseIn", -125.0, 0.0, 270.0, 5.0)
AddSerialCommand("camera", "Orient Relative EaseInAndOut", 10.0, 80.0, 0.0, 5.0)
AddSerialCommand("camera", "Wait Time", 2.0)
CompleteCommandGroup()

CreateCommandGroup()
AddParallelCommand("rubble1", "AddVelocity", 0.0, -1.0, 0.0, 1.0, 1.5, 5.0)
AddParallelCommand("rubble2", "AddVelocity", 0.0, -1.0, 0.0, 0.5, 1.5, 5.0)
AddParallelCommand("camera","Follow Curve CubicBezier",
	-125.0, 0.0, 270.0,
	-125.0, 0.0, 400.0,
	520.0, 0.0, 390.0,
	520.0, 0.0, 370.0,
	10.0
)
AddParallelCommand("camera", "Orient Relative EaseInAndOut", 0.0, 90.0, 0.0, 4.0)
CompleteCommandGroup()

CreateCommandGroup()
AddParallelCommand("camera", "LookAt EaseIn beholder", 2.0)
AddSerialCommand("beholder", "LookAt EaseIn camera", 2.0)
AddSerialCommand("beholder", "TurnLight On", 1.0)  
CompleteCommandGroup()