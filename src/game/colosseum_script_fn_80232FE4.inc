    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r4
    mr r31, r3
    mr r30, r5
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80233014
    li r3, 0x1
    b @L_80233190
@L_80233014:
    cmplw r31, r30
    bne @L_80233024
    li r3, 0x1
    b @L_80233190
@L_80233024:
    mr r3, r31
    li r4, 0x16
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80233070
    mr r3, r31
    li r4, 0x16
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80233070
    mr r4, r29
    bl fn_801F00D0
    cmplwi r3, 0x0
    beq @L_80233070
    cmplw r3, r30
    bne @L_80233070
    li r0, 0x1
    b @L_80233074
@L_80233070:
    li r0, 0x0
@L_80233074:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023308C
    mr r3, r31
    li r4, 0x16
    bl fn_80202810
@L_8023308C:
    mr r3, r31
    li r4, 0xa
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802330D8
    mr r3, r31
    li r4, 0xa
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_802330D8
    mr r4, r29
    bl fn_801F00D0
    cmplwi r3, 0x0
    beq @L_802330D8
    cmplw r3, r30
    bne @L_802330D8
    li r0, 0x1
    b @L_802330DC
@L_802330D8:
    li r0, 0x0
@L_802330DC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802330F4
    mr r3, r31
    li r4, 0xa
    bl fn_80202810
@L_802330F4:
    mr r3, r31
    li r4, 0xe
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80233140
    mr r3, r31
    li r4, 0xe
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80233140
    mr r4, r29
    bl fn_801F00D0
    cmplwi r3, 0x0
    beq @L_80233140
    cmplw r3, r30
    bne @L_80233140
    li r0, 0x1
    b @L_80233144
@L_80233140:
    li r0, 0x0
@L_80233144:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023315C
    mr r3, r31
    li r4, 0xe
    bl fn_80202810
@L_8023315C:
    mr r3, r31
    mr r4, r30
    bl fn_80202B88
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023318C
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    bl fn_80201340
    mr r3, r31
    mr r4, r30
    bl fn_80203198
@L_8023318C:
    li r3, 0x1
@L_80233190:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
