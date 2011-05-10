--����Ľ�������
function Task_Accept_00031905()
	local player = GetPlayer();
	if player:GetLev() < 72 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31905) or task:HasCompletedTask(31905) or task:HasSubmitedTask(31905) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31904) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31904) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031905()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 72 then
		return false;
	end
	if task:HasAcceptedTask(31905) or task:HasCompletedTask(31905) or task:HasSubmitedTask(31905) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31904) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31904) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031905()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31905) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031905(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31905) == npcId and Task_Accept_00031905 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31905
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "吕布的复活";
	elseif task:GetTaskSubmitNpc(31905) == npcId then
		if Task_Submit_00031905() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31905
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "吕布的复活";
		elseif task:HasAcceptedTask(31905) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31905
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "吕布的复活";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031905_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "离滑沙古城不远的月亮湾，是一个得天独厚的幽静的地方，适合在那里使用还魂阵法将吕布复活而不受打扰。";
	action.m_ActionMsg = "好的，我这就去月亮湾复活他。";
	return action;
end

function Task_00031905_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如果整个过程顺利，吕布就可以在常泉寺复活了，完成复活仪式以后，你去那里看看他吧。";
	action.m_ActionMsg = "这次要是没有你，我真就蒙受冤屈了。";
	return action;
end

function Task_00031905_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是貂蝉让你这样做的？居然还能追回自己的灵魂，回到这个真实的世界。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031905_step_table = {
		[1] = Task_00031905_step_01,
		[2] = Task_00031905_step_02,
		[10] = Task_00031905_step_10,
		};

function Task_00031905_step(step)
	if Task_00031905_step_table[step] ~= nil then
		return Task_00031905_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031905_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031905() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15077, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(31905) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00031905_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15077,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(31905) then
		return false;
	end

	package:DelItemAll(15077,1);

	player:AddExp(260000);
	player:getCoin(245000);
	player:getTael(50);
	return true;
end

--��������
function Task_00031905_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15077,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31905);
end
