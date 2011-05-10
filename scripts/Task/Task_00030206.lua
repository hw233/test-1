--����Ľ�������
function Task_Accept_00030206()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 32 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30206) or task:HasCompletedTask(30206) or task:HasSubmitedTask(30206) then
		return false;
	end
	if not task:HasSubmitedTask(30205) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030206()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 32 then
		return false;
	end
	if task:HasAcceptedTask(30206) or task:HasCompletedTask(30206) or task:HasSubmitedTask(30206) then
		return false;
	end
	if not task:HasSubmitedTask(30205) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00030206()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30206) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030206(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30206) == npcId and Task_Accept_00030206 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30206
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "回击夜摩";
	elseif task:GetTaskSubmitNpc(30206) == npcId then
		if Task_Submit_00030206() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30206
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "回击夜摩";
		elseif task:HasAcceptedTask(30206) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30206
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "回击夜摩";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030206_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "山海大陆这里的环境非常险恶，不仅有那些失去理智的天族复仇者的威胁，还有很多邪恶的夜摩盟势力，自从他们的首领夜叉王和摩可拿逃到山海大陆，他们就开始到处收服各种势力，发展自己的规模。";
	action.m_ActionMsg = "那这周围是不是也有很多夜摩盟的势力？";
	return action;
end

function Task_00030206_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是的，我们这些驻守边防的士兵，由于人手有限，而且距离地界遥远，没有援兵，经常受到那些邪恶势力的攻击，尤其是哨所外巨石堆的巨型狼人，我们的士兵有的因此受伤，您能帮助我们去打败那些夜摩盟的狼人吗？我们会非常感激。";
	action.m_ActionMsg = "请放心，我定能将他们杀得片甲不留.";
	return action;
end

function Task_00030206_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "没有了那些狼人，我们的防御工事的威胁也减少了，夜摩盟的邪恶阴谋也无法得逞了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030206_step_table = {
		[1] = Task_00030206_step_01,
		[2] = Task_00030206_step_02,
		[10] = Task_00030206_step_10,
		};

function Task_00030206_step(step)
	if Task_00030206_step_table[step] ~= nil then
		return Task_00030206_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030206_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030206() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30206) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030206_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30206) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end

	player:AddExp(4500);
	player:getCoin(5900);
	player:getTael(10);
	return true;
end

--��������
function Task_00030206_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30206);
end
