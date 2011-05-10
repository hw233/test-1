--����Ľ�������
function Task_Accept_00040703()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40703) or task:HasCompletedTask(40703) or task:HasSubmitedTask(40703) then
		return false;
	end
	if not task:HasSubmitedTask(40702) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040703()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(40703) or task:HasCompletedTask(40703) or task:HasSubmitedTask(40703) then
		return false;
	end
	if not task:HasSubmitedTask(40702) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040703()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40703) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040703(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40703) == npcId and Task_Accept_00040703 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40703
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "传递平安";
	elseif task:GetTaskSubmitNpc(40703) == npcId then
		if Task_Submit_00040703() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40703
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "传递平安";
		elseif task:HasAcceptedTask(40703) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40703
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "传递平安";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040703_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "当初我来到天庭的时候，只认识婆修一个人，他是一个不仅功力高深，为人也很有魅力的人，牛霍州这里最近麻烦不断，但是在勇士你的帮助下，麻烦已经被摆平了，所以能不能麻烦你去告知婆修一下，我这里的麻烦已经解决了。";
	action.m_ActionMsg = "已经帮了你很多了，所以也不在乎再多跑一次了。";
	return action;
end

function Task_00040703_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是太好了，我要享用这稀世的山茶了，不过也请你顺便带一点山茶叶去送给婆修吧，我相信他也一定会喜欢的。";
	action.m_ActionMsg = "好的，不过我觉得猎杀怪物比喝茶有趣多了.";
	return action;
end

function Task_00040703_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "谢谢你送来的好消息勇士，而且还有这味道沁人心脾的山茶叶。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040703_step_table = {
		[1] = Task_00040703_step_01,
		[2] = Task_00040703_step_02,
		[10] = Task_00040703_step_10,
		};

function Task_00040703_step(step)
	if Task_00040703_step_table[step] ~= nil then
		return Task_00040703_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040703_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040703() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15053, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(40703) then
		return false;
	end
	task:AddTaskStep(40703);
	return true;
end



--�ύ����
function Task_00040703_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15053,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8927,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40703) then
		return false;
	end

	if IsEquipTypeId(8927) then
		for k = 1, 1 do
			package:AddEquip(8927, 1);
		end
	else 
		package:AddItem(8927,1,1);
	end
	package:DelItemAll(15053,1);

	player:AddExp(1800);
	player:getCoin(3600);
	return true;
end

--��������
function Task_00040703_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15053,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40703);
end
