    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    mr r8, r4
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r6
    mr r29, r7
    add r30, r8, r5
    b @L_800DA6D4
    @L_800DA5A0:
    lbz r0, 0x0(r8)
    addi r8, r8, 0x1
    rlwinm r0, r0, 0, 24, 28
    cmpwi r0, 0x98
    beq @L_800DA618
    bge @L_800DA5E8
    cmpwi r0, 0x80
    beq @L_800DA618
    bge @L_800DA5DC
    cmpwi r0, 0x61
    beq @L_800DA6C8
    bge @L_800DA6DC
    cmpwi r0, 0x0
    beq @L_800DA6D4
    b @L_800DA6DC
    @L_800DA5DC:
    cmpwi r0, 0x90
    beq @L_800DA618
    b @L_800DA6DC
    @L_800DA5E8:
    cmpwi r0, 0xb0
    beq @L_800DA618
    bge @L_800DA60C
    cmpwi r0, 0xa8
    beq @L_800DA618
    bge @L_800DA6DC
    cmpwi r0, 0xa0
    beq @L_800DA618
    b @L_800DA6DC
    @L_800DA60C:
    cmpwi r0, 0xb8
    beq @L_800DA618
    b @L_800DA6DC
    @L_800DA618:
    cmpwi r0, 0xa0
    lhz r6, 0x0(r8)
    addi r8, r8, 0x2
    beq @L_800DA69C
    bge @L_800DA650
    cmpwi r0, 0x90
    beq @L_800DA68C
    bge @L_800DA644
    cmpwi r0, 0x80
    beq @L_800DA6A4
    b @L_800DA6A8
    @L_800DA644:
    cmpwi r0, 0x98
    beq @L_800DA694
    b @L_800DA6A8
    @L_800DA650:
    cmpwi r0, 0xb0
    beq @L_800DA684
    bge @L_800DA668
    cmpwi r0, 0xa8
    beq @L_800DA67C
    b @L_800DA6A8
    @L_800DA668:
    cmpwi r0, 0xb8
    beq @L_800DA674
    b @L_800DA6A8
    @L_800DA674:
    li r31, 0x0
    b @L_800DA6A8
    @L_800DA67C:
    li r31, 0x1
    b @L_800DA6A8
    @L_800DA684:
    li r31, 0x2
    b @L_800DA6A8
    @L_800DA68C:
    li r31, 0x3
    b @L_800DA6A8
    @L_800DA694:
    li r31, 0x4
    b @L_800DA6A8
    @L_800DA69C:
    li r31, 0x5
    b @L_800DA6A8
    @L_800DA6A4:
    li r31, 0x6
    @L_800DA6A8:
    mr r5, r8
    mr r3, r31
    mr r4, r27
    mr r7, r28
    mr r8, r29
    bl fn_800DA6F0
    mr r8, r3
    b @L_800DA6D4
    @L_800DA6C8:
    addi r8, r8, 0x4
    b @L_800DA6D4
    b @L_800DA6DC
    @L_800DA6D4:
    cmplw r8, r30
    blt @L_800DA5A0
    @L_800DA6DC:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
