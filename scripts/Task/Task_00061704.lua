--����Ľ�������
function Task_Accept_00061704()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61704) or task:HasCompletedTask(61704) or task:HasSubmitedTask(61704) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61703) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61703) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061704()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(61704) or task:HasCompletedTask(61704) or task:HasSubmitedTask(61704) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61703) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61703) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061704()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61704) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061704(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61704) == npcId and Task_Accept_00061704 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61704
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "地冻天寒";
	elseif task:GetTaskSubmitNpc(61704) == npcId then
		if Task_Submit_00061704() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61704
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "地冻天寒";
		elseif task:HasAcceptedTask(61704) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61704
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "地冻天寒";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061704_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这寒冷的地方，没有酒可怎么熬啊，梅犹前这个无赖，不还我钱，我也没办法买酒了，原来存的几瓶酒，也被一些凶恶的怪物抢走了。勇士，你忍心看着我就这么冻死吗？";
	action.m_ActionMsg = "那我能有什么办法？你说有人把你的酒抢走了？";
	return action;
end

function Task_00061704_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是啊，我晕晕乎乎的，也没看清是些什么，就知道抢了我的酒，就跑向灵山塔方向了，我是肯定打不过他们的了，要不勇士你去帮我杀掉他们看看？";
	action.m_ActionMsg = "这我倒是可以去试试看。";
	return action;
end

function Task_00061704_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "怎么没有把我的酒抢回来？难道已经被喝掉了？完了，这地冻天寒的日子没有酒可怎么过啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061704_step_table = {
		[1] = Task_00061704_step_01,
		[2] = Task_00061704_step_02,
		[10] = Task_00061704_step_10,
		};

function Task_00061704_step(step)
	if Task_00061704_step_table[step] ~= nil then
		return Task_00061704_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061704_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061704() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61704) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061704_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8906,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61704) then
		return false;
	end

	if IsEquipTypeId(8906) then
		for k = 1, 1 do
			package:AddEquip(8906, 1);
		end
	else 
		package:AddItem(8906,1,1);
	end

	player:AddExp(110000);
	player:getCoin(130000);
	return true;
end

--��������
function Task_00061704_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61704);
end
