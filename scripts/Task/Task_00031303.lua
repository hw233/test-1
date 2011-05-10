--����Ľ�������
function Task_Accept_00031303()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31303) or task:HasCompletedTask(31303) or task:HasSubmitedTask(31303) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31302) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31302) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(31303) or task:HasCompletedTask(31303) or task:HasSubmitedTask(31303) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31302) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31302) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31303) == npcId and Task_Accept_00031303 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "设置埋伏";
	elseif task:GetTaskSubmitNpc(31303) == npcId then
		if Task_Submit_00031303() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31303
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "设置埋伏";
		elseif task:HasAcceptedTask(31303) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31303
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "设置埋伏";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你做的太好了，通过最近的暗查，我不仅发现了摩睺也加入了夜摩盟的消息，而且你也向我们传递了最近摩可拿从原来的藏身之地逃走的消息，并且据我推测他很有可能从摩加湖的南湖冰道逃往西边。";
	action.m_ActionMsg = "是的。";
	return action;
end

function Task_00031303_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "所以我想请你帮忙在南湖冰道上布置毒刺埋伏，如果摩可拿真的从那里经过，可以帮助我们更好的阻止他。";
	action.m_ActionMsg = "没问题，这是很好的办法。";
	return action;
end

function Task_00031303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "有了这样的布防，相信我们可以更好的阻止夜摩盟，至少也是在时间上对他们进行拖延。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031303_step_table = {
		[1] = Task_00031303_step_01,
		[2] = Task_00031303_step_02,
		[10] = Task_00031303_step_10,
		};

function Task_00031303_step(step)
	if Task_00031303_step_table[step] ~= nil then
		return Task_00031303_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031303() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15069, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(31303) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00031303_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15069,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(705,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31303) then
		return false;
	end

	if IsEquipTypeId(705) then
		for k = 1, 1 do
			package:AddEquip(705, 1);
		end
	else 
		package:AddItem(705,1,1);
	end
	package:DelItemAll(15069,1);

	player:AddExp(110000);
	player:getCoin(107500);
	player:getTael(40);
	return true;
end

--��������
function Task_00031303_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15069,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31303);
end
