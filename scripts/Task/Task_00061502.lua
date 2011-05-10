--����Ľ�������
function Task_Accept_00061502()
	local player = GetPlayer();
	if player:GetLev() < 58 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61502) or task:HasCompletedTask(61502) or task:HasSubmitedTask(61502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61501) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061502()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 58 then
		return false;
	end
	if task:HasAcceptedTask(61502) or task:HasCompletedTask(61502) or task:HasSubmitedTask(61502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61501) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061502()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61502) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061502(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61502) == npcId and Task_Accept_00061502 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61502
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "抢回门板";
	elseif task:GetTaskSubmitNpc(61502) == npcId then
		if Task_Submit_00061502() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61502
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "抢回门板";
		elseif task:HasAcceptedTask(61502) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61502
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "抢回门板";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061502_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士就在你刚刚离开去消除骷髅恶龙的这段时间，冰垂林那边冲来了一群食尸巨人，他们摧毁了渡冰园的建筑，抢走了我住所放间的门板，然后想要把我打死，幸好我躲到了一个夹缝了才活了下来。";
	action.m_ActionMsg = "这么惊险？不过你没有遇到危险就好。";
	return action;
end

function Task_00061502_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士还得麻烦你去把我的门板抢回来，冰凌城这么冷的地方，没有门板，我的房子就太冷了，没法过了。";
	action.m_ActionMsg = "真是什么怪事都有，还有抢门板的怪物。";
	return action;
end

function Task_00061502_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哦门板回来了，真是太好了，渡冰园需要一番修理了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061502_step_table = {
		[1] = Task_00061502_step_01,
		[2] = Task_00061502_step_02,
		[10] = Task_00061502_step_10,
		};

function Task_00061502_step(step)
	if Task_00061502_step_table[step] ~= nil then
		return Task_00061502_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061502_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061502() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61502) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15014,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15014, itemNum, 1);
		end
		task:AddTaskStep2(61502, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00061502_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15014,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(61502) then
		return false;
	end

	package:DelItemAll(15014,1);

	player:AddExp(88000);
	player:getCoin(90000);
	return true;
end

--��������
function Task_00061502_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15014,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61502);
end
