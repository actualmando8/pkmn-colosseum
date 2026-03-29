    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r0, lbl_8047A31C(r13)
    cmpwi r0, 0x4
    bge @L_8001F2AC
    li r3, 0x0
    bl fn_801337A8
    cmplwi r31, 0x0
    beq @L_8001F2F0
    lwz r3, 0x4(r31)
    bl fn_8005DA18
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    andi. r0, r0, 0x1100
    cmplwi r0, 0x0
    beq @L_8001F264
    li r0, 0x4
    li r3, 0x46e
    stw r0, lbl_8047A31C(r13)
    li r4, 0x0
    li r5, 0x0
    bl fn_801669E4
    li r0, 0x1
    stw r0, lbl_8047A328(r13)
    @L_8001F264:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 22, 22
    cmplwi r0, 0x0
    beq @L_8001F2F0
    li r0, 0x4
    li r3, 0x46e
    stw r0, lbl_8047A31C(r13)
    li r4, 0x0
    li r5, 0x0
    bl fn_801669E4
    li r0, 0x1
    stw r0, lbl_8047A328(r13)
    b @L_8001F2F0
    @L_8001F2AC:
    li r3, 0x1
    bl fn_801337A8
    cmplwi r31, 0x0
    beq @L_8001F2F0
    lwz r3, 0x4(r31)
    bl fn_8005DA18
    bl fn_80105624
    lhz r0, 0x4(r3)
    andi. r0, r0, 0x810
    cmpwi r0, 0x0
    beq @L_8001F2F0
    li r0, 0x1
    li r3, 0x4c2
    stb r0, 0x98(r31)
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    @L_8001F2F0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
