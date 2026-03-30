    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    bl fn_800F7AF0
    mr r31, r3
    mr r3, r30
    bl fn_800F7BC4
    and r31, r3, r31
    cmplwi r31, 0x0
    bne @L_800712FC
    mr r3, r30
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800712FC
    mr r3, r30
    bl fn_8006B154
    addi r4, r1, 0x8
    bl fn_80073A44
    cmpwi r3, 0x0
    bne @L_800712FC
    lhz r3, 0x8(r1)
    rlwinm r0, r3, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_8007127C
    ori r31, r31, 0x100
@L_8007127C:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8007128C
    ori r31, r31, 0x200
@L_8007128C:
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8007129C
    ori r31, r31, 0x1000
@L_8007129C:
    rlwinm r0, r3, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_800712AC
    ori r31, r31, 0x2
@L_800712AC:
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_800712BC
    ori r31, r31, 0x1
@L_800712BC:
    rlwinm r0, r3, 0, 25, 25
    cmpwi r0, 0x0
    beq @L_800712CC
    ori r31, r31, 0x8
@L_800712CC:
    rlwinm r0, r3, 0, 24, 24
    cmpwi r0, 0x0
    beq @L_800712DC
    ori r31, r31, 0x4
@L_800712DC:
    rlwinm r0, r3, 0, 23, 23
    cmpwi r0, 0x0
    beq @L_800712EC
    ori r31, r31, 0x20
@L_800712EC:
    rlwinm r0, r3, 0, 22, 22
    cmpwi r0, 0x0
    beq @L_800712FC
    ori r31, r31, 0x40
@L_800712FC:
    mr r3, r31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
