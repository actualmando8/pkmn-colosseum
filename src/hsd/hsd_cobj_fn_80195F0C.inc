    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    mr r31, r3
    lwz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 30, 30
    bne @L_801960B0
    lwz r0, 0x8(r31)
    li r4, 0x1
    li r5, 0x1
    rlwinm. r0, r0, 0, 1, 1
    bne @L_80195F70
    lwz r0, 0x24(r31)
    li r6, 0x0
    cmplwi r0, 0x0
    beq @L_80195F64
    lwz r3, 0x24(r31)
    lwz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_80195F64
    li r6, 0x1
@L_80195F64:
    cmpwi r6, 0x0
    bne @L_80195F70
    li r5, 0x0
@L_80195F70:
    cmpwi r5, 0x0
    bne @L_80195FA8
    lwz r0, 0x28(r31)
    li r5, 0x0
    cmplwi r0, 0x0
    beq @L_80195F9C
    lwz r3, 0x28(r31)
    lwz r0, 0x8(r3)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_80195F9C
    li r5, 0x1
@L_80195F9C:
    cmpwi r5, 0x0
    bne @L_80195FA8
    li r4, 0x0
@L_80195FA8:
    cmpwi r4, 0x0
    beq @L_801960B0
    cmplwi r31, 0x0
    bne @L_80195FC8
    la r3, -23880(r2)
    li r4, 0x318
    la r5, -23872(r2)
    bl __assert
@L_80195FC8:
    cmplwi r31, 0x0
    bne @L_80195FE0
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_80195FE0:
    lwz r3, 0x24(r31)
    addi r4, r1, 0x20
    bl fn_80191688
    mr r3, r31
    addi r4, r1, 0x14
    bl HSD_CObjGetUpVector
    cmpwi r3, 0x0
    bne @L_80196018
    lfs f2, -23848(r2)
    lfs f1, -23792(r2)
    lfs f0, -23848(r2)
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
@L_80196018:
    cmplwi r31, 0x0
    bne @L_80196030
    la r3, -23880(r2)
    li r4, 0x300
    la r5, -23872(r2)
    bl __assert
@L_80196030:
    cmplwi r31, 0x0
    bne @L_80196048
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_80196048:
    lwz r3, 0x28(r31)
    addi r4, r1, 0x8
    bl fn_80191688
    addi r3, r31, 0x54
    addi r4, r1, 0x20
    addi r5, r1, 0x14
    addi r6, r1, 0x8
    bl C_MTXLookAt
    lwz r3, 0x24(r31)
    cmplwi r31, 0x0
    lwz r0, 0x8(r3)
    rlwinm r0, r0, 0, 31, 29
    stw r0, 0x8(r3)
    lwz r3, 0x28(r31)
    lwz r0, 0x8(r3)
    rlwinm r0, r0, 0, 31, 29
    stw r0, 0x8(r3)
    beq @L_8019609C
    lwz r0, 0x8(r31)
    rlwinm r0, r0, 0, 2, 0
    stw r0, 0x8(r31)
@L_8019609C:
    cmplwi r31, 0x0
    beq @L_801960B0
    lwz r0, 0x8(r31)
    oris r0, r0, 0x8000
    stw r0, 0x8(r31)
@L_801960B0:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
