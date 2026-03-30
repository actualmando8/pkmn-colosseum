    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r5
    stw r30, 0x8(r1)
    mr r30, r4
    bne @L_80134A0C
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_80134A0C:
    extsb. r0, r30
    blt @L_80134A20
    extsb r0, r30
    cmpwi r0, 0x3
    blt @L_80134A28
    @L_80134A20:
    li r3, 0x0
    b @L_80134A80
    @L_80134A28:
    cmplwi r31, 0x0
    bne @L_80134A38
    li r3, 0x0
    b @L_80134A80
    @L_80134A38:
    mr r5, r31
    li r4, 0x0
    b @L_80134A4C
    @L_80134A44:
    addi r5, r5, 0x2
    addi r4, r4, 0x1
    @L_80134A4C:
    lhz r0, 0x0(r5)
    cmplwi r0, 0x0
    bne @L_80134A44
    cmpwi r4, 0x8
    ble @L_80134A68
    li r3, 0x0
    b @L_80134A80
    @L_80134A68:
    extsb r0, r30
    mr r4, r31
    mulli r0, r0, 0x24a4
    add r3, r3, r0
    bl fn_800F9E70
    li r3, 0x1
    @L_80134A80:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
