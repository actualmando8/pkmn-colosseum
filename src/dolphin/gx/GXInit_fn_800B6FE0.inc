    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x2e0(r1)
    stw r31, 0x2dc(r1)
    mr r31, r4
    lwz r5, -24228(r13)
    lwz r3, -29632(r13)
    lhz r0, 0x0(r5)
    stw r0, 0xc(r3)
    lwz r0, 0x8(r3)
    rlwinm. r0, r0, 29, 31, 31
    beq @L_800B7044
    lwz r0, 0xc(r3)
    rlwinm. r0, r0, 31, 31, 31
    beq @L_800B7044
    lwz r3, -24184(r13)
    bl fn_800A1F94
    li r0, 0x0
    stw r0, -24176(r13)
    li r3, 0x1
    li r4, 0x1
    bl fn_800B75D0
    li r3, 0x1
    li r4, 0x0
    bl fn_800B7594
@L_800B7044:
    lwz r3, -29632(r13)
    lwz r0, 0x8(r3)
    rlwinm. r0, r0, 30, 31, 31
    beq @L_800B7094
    lwz r0, 0xc(r3)
    rlwinm. r0, r0, 0, 31, 31
    beq @L_800B7094
    lwz r5, -24168(r13)
    li r3, 0x0
    li r4, 0x1
    addi r0, r5, 0x1
    stw r0, -24168(r13)
    bl fn_800B7594
    li r3, 0x1
    li r4, 0x0
    bl fn_800B75D0
    li r0, 0x1
    lwz r3, -24184(r13)
    stw r0, -24176(r13)
    bl fn_800A221C
@L_800B7094:
    lwz r3, -29632(r13)
    lwz r4, 0x8(r3)
    addi r5, r3, 0x8
    rlwinm. r0, r4, 27, 31, 31
    beq @L_800B7100
    lwz r0, 0xc(r3)
    rlwinm. r0, r0, 28, 31, 31
    beq @L_800B7100
    rlwinm r0, r4, 0, 27, 25
    stw r0, 0x0(r5)
    lwz r0, 0x8(r3)
    lwz r3, -24228(r13)
    sth r0, 0x2(r3)
    lwz r0, -24172(r13)
    cmplwi r0, 0x0
    beq @L_800B7100
    addi r3, r1, 0x10
    bl OSClearContext
    addi r3, r1, 0x10
    bl OSSetCurrentContext
    lwz r12, -24172(r13)
    mtlr r12
    blrl
    addi r3, r1, 0x10
    bl OSClearContext
    mr r3, r31
    bl OSSetCurrentContext
@L_800B7100:
    lwz r0, 0x2e4(r1)
    lwz r31, 0x2dc(r1)
    addi r1, r1, 0x2e0
    mtlr r0
    blr
