--����Ľ�������
function Task_Accept_00000614()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(614) or task:HasCompletedTask(614) or task:HasSubmitedTask(614) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000614()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(614) or task:HasCompletedTask(614) or task:HasSubmitedTask(614) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000614()
	if GetPlayer():GetTaskMgr():HasCompletedTask(614) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000614(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(614) == npcId and Task_Accept_00000614 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 614
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "福仙潭毒石";
	elseif task:GetTaskSubmitNpc(614) == npcId then
		if Task_Submit_00000614() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 614
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "福仙潭毒石";
		elseif task:HasAcceptedTask(614) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 614
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "福仙潭毒石";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000614_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成都不远处的桂花山是一处美景，不过山中有五色云雾，寻常人轻易不得入内，听说有个室外高人隐居在山中，布下了五色云雾。在桂花山里有一处福仙潭，福仙潭里水产丰富，可是福仙潭底有一个巨大毒石，毒雾缭绕，有不少渔民被毒雾害死，还拜托少侠你除掉这一害。";
	action.m_ActionMsg = "这种事情我还是第一次听说，我去看看顺手将它除掉。";
	return action;
end

function Task_00000614_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这个毒石剧毒无比，毒害了很多在福仙潭的渔夫旅。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000614_step_table = {
		[1] = Task_00000614_step_01,
		[10] = Task_00000614_step_10,
		};

function Task_00000614_step(step)
	if Task_00000614_step_table[step] ~= nil then
		return Task_00000614_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000614_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000614() then
		return false;
	end
	if not task:AcceptTask(614) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000614_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(614) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--��������
function Task_00000614_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(614);
end
